dnl Check for standard types

AC_DEFUN(OSIS_CHECK_TYPES,
[ AC_CHECK_SIZEOF(short, 2)
  if test x$ac_cv_sizeof_short = x2; then
    S_INT16_T="signed short"
    U_INT16_T="unsigned short"
  else
    AC_MSG_WARN("No 16 bit type used. Using int instead. This might break the package.")
    S_INT16_T="signed int"
    U_INT16_T="unsigned int"
  fi

  AC_SUBST(S_INT16_T)
  AC_SUBST(U_INT16_T)
])
