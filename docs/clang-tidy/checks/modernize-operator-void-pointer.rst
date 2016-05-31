.. title:: clang-tidy - modernize-operator-void-pointer

modernize-operator-void-pointer
================================

This check finds implicit ``operator const void*`` overloads and replaces them
with ``explicit operator bool`` overloads, available since C++11.

Implicit ``operator const void*`` overloads were often used before C++11 to
avoid implicit conversions to ``bool`` when providing an ``operator bool``
overload, but C++11 provides the ``explicit`` keyword.

See also the modernize-implicit-operator-bool check.

.. code-block:: c++

  operator void* () const;

  // becomes

  explicit operator bool () const;
