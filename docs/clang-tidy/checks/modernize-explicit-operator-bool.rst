.. title:: clang-tidy - modernize-explicit-operator-bool

modernize-explicit-operator-bool
================================

This check finds implicit ``operator bool`` overloads and inserts the
``explicit`` keyword, which is available since C++11.

Without the ``explicit`` keyword, the implicit ``bool`` overload can allow
objects to be compared accidentally. For instance, even when objects `a` and
`b` have no ``operator ==`` overloads, an implicit ``operator bool`` would allow
`a == b` to compile because both `a` and `b` can be implictly converted to
``bool``.

This check also finds implicit ``operator const void*`` overloads. These were
often used before C++11 to avoid implicit conversions to ``bool`` when providing
an ``operator bool`` overload.

To disable the check for ``operator const void*`` overloads, you may set
The :option:`WarnOnOperatorVoidPointer` option to 1.

.. code-block:: c++

  operator bool () const;

  // becomes

  explicit operator bool () const;
