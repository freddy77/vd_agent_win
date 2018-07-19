AC_DEFUN([ACX_PUSH_VAR],
[m4_pushdef([SAVE$1],save_$1[_]__line__)dnl
SAVE$1="$$1"
$1=[$2]])

AC_DEFUN([ACX_POP_VAR],
[$1="$SAVE$1"
unset SAVE$1 dnl
m4_popdef([SAVE$1])dnl
])

AC_DEFUN([ACX_SET_VAR],
[ACX_PUSH_VAR($1, [$2])
[$3]
ACX_POP_VAR($1)])
