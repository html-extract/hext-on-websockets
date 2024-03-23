#include "SetupSeccomp.h"

#include <seccomp.h>
#include <sys/ioctl.h>

#include <memory>
#include <sstream>


#define WS_SYS_PAIR(ARG) { SCMP_SYS(ARG), #ARG }


namespace {


std::string ErrorString(const char * desc, int rc)
{
  std::ostringstream os;
  os << desc << " (" << rc << ")";
  return os.str();
}

std::string RuleAddErrorString(const char * sys_name, int sys_value, int rc)
{
  std::ostringstream os;
  os << "seccomp_rule_add allow " << sys_name << " (" << sys_value
     << ") failed (" << rc << ")";
  return os.str();
}


} // namespace


namespace ws {


void SetupSeccomp()
{
  // scope guard for seccomp_{init,release}
  using SeccompGuard = std::unique_ptr<void, decltype(&seccomp_release)>;
  SeccompGuard ctx(seccomp_init(SCMP_ACT_KILL_PROCESS), seccomp_release);

  if( !ctx )
    throw SetupSeccompError("seccomp_init failed");

  std::pair<int, const char *> whitelist[] = {
    WS_SYS_PAIR(accept),
    WS_SYS_PAIR(brk),
    WS_SYS_PAIR(close),
    WS_SYS_PAIR(epoll_ctl),
    WS_SYS_PAIR(epoll_wait),
    WS_SYS_PAIR(futex),
    WS_SYS_PAIR(getpid),
    WS_SYS_PAIR(getrandom),
    WS_SYS_PAIR(madvise),
    WS_SYS_PAIR(mmap),
    WS_SYS_PAIR(mprotect),
    WS_SYS_PAIR(munmap),
    WS_SYS_PAIR(read),
    WS_SYS_PAIR(recvmsg),
    WS_SYS_PAIR(rseq),
    WS_SYS_PAIR(sendmsg),
    WS_SYS_PAIR(shutdown),
    WS_SYS_PAIR(timerfd_settime),
  };

  for( const auto& [sys_value, sys_name] : whitelist )
    if( int rc = seccomp_rule_add(ctx.get(), SCMP_ACT_ALLOW, sys_value, 0) )
      throw SetupSeccompError(RuleAddErrorString(sys_name, sys_value, rc));

  // glibc v2.37: `free` uses the `openat` syscall to open `/proc/sys/vm/overcommit_memory`.
  //   Failing with EACCES will make `free` behave as if overcommit_memory is enabled,
  //   which is the default in most systems.
  // Note: Comparing strings in seccomp is considered insecure because of TOCTOU race
  //   conditions.
  if( int rc = seccomp_rule_add(ctx.get(), SCMP_ACT_ERRNO(EACCES), SCMP_SYS(openat), 0) )
    throw SetupSeccompError(
        ErrorString("seccomp_rule_add SCMP_ACT_ERRNO(EACCES) failed for openat", rc));

  // Allow ioctl only with flag FIONBIO, i.e. `ioctl(any, FIONBIO, any)`.
  if( int rc = seccomp_rule_add(ctx.get(),
                                SCMP_ACT_ALLOW,
                                SCMP_SYS(ioctl),
                                1,
                                SCMP_A1_64(SCMP_CMP_EQ,
                                           static_cast<scmp_datum_t>(FIONBIO))) )
    throw SetupSeccompError(ErrorString("seccomp_rule_add allow failed for ioctl with FIONBIO", rc));

  if( int rc = seccomp_load(ctx.get()) )
    throw SetupSeccompError(ErrorString("seccomp_load failed", rc));
}


} // namespace ws

