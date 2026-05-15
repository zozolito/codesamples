# Fortran — Reference Manual

> A practical reference for modern Fortran (Fortran 90/95/2003/2008/2018).  
> All examples compile with `gfortran -Wall -fcheck=all source.f90`.

---

## 🇬🇧 English

### Table of Contents

1. [Source Format](#1-source-format)
2. [Program Structure](#2-program-structure)
3. [Data Types](#3-data-types)
   - [Intrinsic types](#31-intrinsic-types)
   - [KIND — precision control](#32-kind--precision-control)
   - [Constants (PARAMETER)](#33-constants-parameter)
   - [Implicit none](#34-implicit-none)
4. [Variables — Declaration and Attributes](#4-variables--declaration-and-attributes)
5. [Operators](#5-operators)
6. [Control Flow](#6-control-flow)
   - [IF / ELSE IF / ELSE](#61-if--else-if--else)
   - [SELECT CASE](#62-select-case)
   - [DO loops](#63-do-loops)
   - [EXIT and CYCLE](#64-exit-and-cycle)
7. [Arrays](#7-arrays)
   - [Declaration](#71-declaration)
   - [Array operations](#72-array-operations)
   - [Allocatable arrays](#73-allocatable-arrays)
   - [Array sections (slicing)](#74-array-sections-slicing)
8. [Characters and Strings](#8-characters-and-strings)
9. [Derived Types (Structs)](#9-derived-types-structs)
10. [Subprograms — Subroutines and Functions](#10-subprograms--subroutines-and-functions)
    - [Subroutines](#101-subroutines)
    - [Functions](#102-functions)
    - [Intent](#103-intent)
    - [Optional arguments](#104-optional-arguments)
    - [Recursive procedures](#105-recursive-procedures)
    - [Internal subprograms (CONTAINS)](#106-internal-subprograms-contains)
11. [Modules](#11-modules)
12. [Pointers](#12-pointers)
13. [Input / Output](#13-input--output)
    - [Formatted I/O](#131-formatted-io)
    - [Format descriptors](#132-format-descriptors)
    - [File I/O](#133-file-io)
14. [Intrinsic Functions](#14-intrinsic-functions)
15. [Preprocessing and Compiler Directives](#15-preprocessing-and-compiler-directives)
16. [Error Handling](#16-error-handling)
17. [Quick Reference Card](#17-quick-reference-card)

---

## 1. Source Format

### Fixed format (Fortran 77 and older)

The historical format used punch-card column rules:

```
Col 1     : 'C' or '*' = comment
Col 1–5   : Statement label (line number)
Col 6     : Continuation marker (any character except '0' or ' ')
Col 7–72  : Statement
Col 73–80 : Identification (ignored)
```

Files use extension `.f` or `.for`.

### Free format (Fortran 90+ — use this)

- No column restrictions
- Lines up to 132 characters (convention; compilers accept more)
- `!` starts a comment (to end of line)
- `&` at the end of a line continues to the next
- Multiple statements on one line separated by `;`
- Files use extension `.f90`, `.f95`, `.f03`, `.f08`

```fortran
! This is a free-format comment
x = 1.0 + &       ! continuation
    2.0            ! next line completes the expression

a = 1; b = 2; c = 3   ! multiple statements per line
```

**Compile fixed-format:**  `gfortran -ffixed-form source.f`  
**Compile free-format:**   `gfortran source.f90` (auto-detected by extension)

---

## 2. Program Structure

### Main program

```fortran
program program_name
    implicit none
    ! declarations
    ! executable statements
contains
    ! internal subprograms (optional)
end program program_name
```

### Complete structure example

```fortran
program solar_system
    use physics_module       ! use a module
    use, intrinsic :: iso_fortran_env, only: real64, int32

    implicit none

    real(real64) :: mass, velocity, energy
    integer(int32) :: planet_count

    mass = 5.972e24_real64   ! Earth mass in kg
    velocity = 29780.0_real64 ! Earth orbital speed m/s
    energy = 0.5_real64 * mass * velocity**2

    write(*, '(A, ES12.4)') 'Kinetic energy (J): ', energy

contains

    subroutine report(label, value)
        character(len=*), intent(in) :: label
        real(real64),     intent(in) :: value
        write(*, '(A, A, ES12.4)') label, ': ', value
    end subroutine report

end program solar_system
```

---

## 3. Data Types

### 3.1 Intrinsic types

| Type | Example declaration | Description |
|---|---|---|
| `INTEGER` | `integer :: n` | Whole numbers |
| `REAL` | `real :: x` | Floating point (single precision by default) |
| `DOUBLE PRECISION` | `double precision :: d` | 64-bit float (legacy syntax) |
| `COMPLEX` | `complex :: z` | Complex number (real + imaginary parts) |
| `LOGICAL` | `logical :: flag` | Boolean: `.true.` or `.false.` |
| `CHARACTER` | `character(len=50) :: name` | Fixed-length string |

```fortran
integer          :: count = 0
real             :: pi = 3.14159265
double precision :: precise = 3.14159265358979_8
complex          :: z = (1.0, -2.0)    ! 1 - 2i
logical          :: done = .false.
character(len=20):: greeting = "Hello"
```

### 3.2 KIND — precision control

`KIND` is the portable, correct way to specify precision.

```fortran
use, intrinsic :: iso_fortran_env, only: &
    int8, int16, int32, int64,            &
    real32, real64, real128

integer(int32)  :: standard_int     ! exactly 32 bits
integer(int64)  :: big_int          ! exactly 64 bits
real(real32)    :: single           ! 32-bit IEEE 754
real(real64)    :: double           ! 64-bit IEEE 754
real(real128)   :: quad             ! 128-bit (where available)

! Legacy KIND selection (still valid)
integer, parameter :: dp = selected_real_kind(15, 307)   ! double precision
integer, parameter :: sp = selected_real_kind(6,  37)    ! single precision
integer, parameter :: i8 = selected_int_kind(18)         ! 64-bit integer

real(dp) :: x = 1.0_dp    ! _dp suffix applies the kind to the literal
```

> Always suffix literals with their kind: `1.0_dp` not `1.0`. Without the suffix, a literal is the default kind regardless of the variable's kind — a common source of precision loss.

### 3.3 Constants (PARAMETER)

```fortran
integer,  parameter :: MAX_SIZE = 1000
real(dp), parameter :: PI  = 3.14159265358979323846_dp
real(dp), parameter :: E   = 2.71828182845904523536_dp
real(dp), parameter :: C   = 299792458.0_dp   ! speed of light m/s
logical,  parameter :: DEBUG = .false.

character(len=*), parameter :: VERSION = "1.0.0"
! len=* means length is determined by the initializer
```

### 3.4 Implicit none

```fortran
implicit none
```

**Always use this. Without it**, Fortran applies the implicit typing rule: variables starting with `i`–`n` are `INTEGER`, everything else is `REAL`. This rule has caused famous bugs in scientific codes. `implicit none` forces every variable to be explicitly declared.

---

## 4. Variables — Declaration and Attributes

```fortran
! Basic declaration
integer :: n
real(dp) :: x, y, z

! With initialization
integer :: counter = 0
real    :: pi = 3.14159265

! SAVE attribute — variable retains value between calls (only in subprograms)
integer, save :: call_count = 0

! PARAMETER — compile-time constant
real, parameter :: GRAVITY = 9.81

! ALLOCATABLE — dynamic memory (see Arrays section)
real, allocatable :: data(:)

! POINTER
real, pointer :: ptr => null()

! TARGET — can be pointed to
real, target :: value = 42.0

! VOLATILE — not cached (useful for hardware registers)
integer, volatile :: hardware_register

! PROTECTED (module variable — readable outside but writable only inside)
real, protected :: internal_state
```

---

## 5. Operators

### Arithmetic operators

| Operator | Meaning | Example |
|---|---|---|
| `+` | Addition | `a + b` |
| `-` | Subtraction | `a - b` |
| `*` | Multiplication | `a * b` |
| `/` | Division | `a / b` |
| `**` | Exponentiation | `a ** b` |

> Integer division truncates: `7 / 2 = 3`. Use `7.0 / 2` or `real(7) / 2` for `3.5`.

### Comparison operators

| Operator | Symbolic | Meaning |
|---|---|---|
| `==` | `.EQ.` | Equal |
| `/=` | `.NE.` | Not equal |
| `<` | `.LT.` | Less than |
| `<=` | `.LE.` | Less than or equal |
| `>` | `.GT.` | Greater than |
| `>=` | `.GE.` | Greater than or equal |

### Logical operators

| Operator | Meaning |
|---|---|
| `.and.` | Both true |
| `.or.` | At least one true |
| `.not.` | Negation |
| `.eqv.` | Logical equivalence |
| `.neqv.` | Logical non-equivalence (XOR) |

### String operator

| Operator | Meaning | Example |
|---|---|---|
| `//` | Concatenation | `"Hello" // ", " // "World"` |

---

## 6. Control Flow

### 6.1 IF / ELSE IF / ELSE

```fortran
! Single-line IF
if (x > 0) write(*, *) 'Positive'

! Block IF
if (x > 0.0) then
    write(*, *) 'Positive'
else if (x < 0.0) then
    write(*, *) 'Negative'
else
    write(*, *) 'Zero'
end if

! Named IF (useful for nested structures)
outer: if (condition_a) then
    inner: if (condition_b) then
        ! ...
    end if inner
end if outer
```

### 6.2 SELECT CASE

```fortran
integer :: day

select case (day)
    case (1)
        write(*, *) 'Monday'
    case (2)
        write(*, *) 'Tuesday'
    case (3:5)
        write(*, *) 'Wednesday to Friday'
    case (6, 7)
        write(*, *) 'Weekend'
    case default
        write(*, *) 'Invalid day'
end select

! Works with characters too
character :: grade
select case (grade)
    case ('A', 'B')
        write(*, *) 'Good'
    case ('C')
        write(*, *) 'Average'
    case default
        write(*, *) 'Below average'
end select
```

### 6.3 DO loops

```fortran
! Counted DO loop
do i = 1, 10
    write(*, *) i
end do

! With step
do i = 1, 100, 5    ! 1, 6, 11, ..., 96
    write(*, *) i
end do

! Count down
do i = 10, 1, -1
    write(*, *) i
end do

! DO WHILE
do while (x > 1.0e-6)
    x = x / 2.0
end do

! Infinite loop (exit with EXIT)
do
    read(*, *) input
    if (input == 0) exit
    call process(input)
end do

! Named DO loop — useful with EXIT and CYCLE in nested loops
outer: do i = 1, n
    inner: do j = 1, m
        if (matrix(i,j) == 0) cycle outer   ! skip to next i
        if (matrix(i,j) < 0) exit outer     ! exit both loops
    end do inner
end do outer
```

### 6.4 EXIT and CYCLE

```fortran
! EXIT — break out of the loop
do i = 1, 100
    if (found) exit   ! leaves the loop immediately
end do

! CYCLE — skip to next iteration
do i = 1, 10
    if (mod(i, 2) == 0) cycle   ! skip even numbers
    write(*, *) i
end do
```

---

## 7. Arrays

### 7.1 Declaration

```fortran
! Fixed-size (rank 1)
integer :: counts(10)           ! indices 1 to 10 by default
real    :: vector(0:99)         ! indices 0 to 99
real    :: grid(-5:5)           ! indices -5 to 5

! Fixed-size (multi-dimensional)
real :: matrix(3, 3)            ! 3×3 matrix
real :: tensor(4, 4, 4)         ! 4×4×4 tensor, rank 3

! Equivalent declaration syntax
integer, dimension(10) :: counts

! Initialization
integer :: primes(5) = [2, 3, 5, 7, 11]
real    :: identity(2,2) = reshape([1.,0.,0.,1.], [2,2])
```

### 7.2 Array operations

Fortran operates on whole arrays without loops — one of its greatest strengths.

```fortran
real :: a(5) = [1.0, 2.0, 3.0, 4.0, 5.0]
real :: b(5) = [10.0, 20.0, 30.0, 40.0, 50.0]
real :: c(5)

! Element-wise arithmetic — no loop needed
c = a + b           ! [11, 22, 33, 44, 55]
c = a * b           ! [10, 40, 90, 160, 250]
c = a ** 2          ! [1, 4, 9, 16, 25]
c = sqrt(a)         ! element-wise sqrt

! Reduction functions
write(*, *) sum(a)      ! 15.0
write(*, *) product(a)  ! 120.0
write(*, *) maxval(a)   ! 5.0
write(*, *) minval(a)   ! 1.0
write(*, *) maxloc(a)   ! [5] (index of max)
write(*, *) minloc(a)   ! [1] (index of min)

! Logical
write(*, *) any(a > 3)  ! .true.
write(*, *) all(a > 0)  ! .true.
write(*, *) count(a > 2)! 3

! Matrix operations
real :: m(3,3), n(3,3), result_m(3,3)
result_m = matmul(m, n)         ! matrix multiplication
write(*, *) dot_product(a, b)   ! vector dot product
write(*, *) transpose(m)        ! matrix transpose

! Reshape
real :: flat(9) = [(real(i), i = 1, 9)]
real :: mat3(3,3)
mat3 = reshape(flat, [3, 3])
```

### 7.3 Allocatable arrays

Allocatable arrays are dynamic — size determined at runtime.

```fortran
real, allocatable :: data(:)        ! rank-1
real, allocatable :: grid(:,:)      ! rank-2
integer, allocatable :: indices(:)

integer :: n = 100

! Allocate
allocate(data(n))
allocate(grid(n, n))
allocate(indices(n), stat=alloc_status)   ! stat= catches errors

if (alloc_status /= 0) then
    write(*, *) 'Allocation failed'
    stop 1
end if

! Use
data = 0.0
data(1) = 42.0

! Check allocation status
if (allocated(data)) write(*, *) 'data is allocated, size:', size(data)

! Resize (deallocate and reallocate)
deallocate(data)
allocate(data(2*n))

! Automatic deallocation when a local allocatable goes out of scope
! (Fortran 95+) — no memory leak for local variables
```

### 7.4 Array sections (slicing)

```fortran
real :: a(10)
a = [(real(i), i = 1, 10)]   ! [1, 2, 3, ..., 10]

! Sections — stride syntax: start:end:step
a(3:7)     ! elements 3, 4, 5, 6, 7
a(1:9:2)   ! elements 1, 3, 5, 7, 9
a(::2)     ! every other element from start to end
a(10:1:-1) ! reversed

! Matrix sections
real :: m(5,5)
m(2:4, 2:4)   ! 3×3 sub-matrix
m(:, 1)       ! entire first column
m(1, :)       ! entire first row
m(1:3, 2)     ! first 3 rows of column 2

! WHERE — conditional array assignment
where (a > 0.0)
    a = sqrt(a)
elsewhere
    a = 0.0
end where
```

---

## 8. Characters and Strings

```fortran
character(len=20) :: first_name
character(len=20) :: last_name
character(len=41) :: full_name
character(len=*)  :: message   ! assumed length — only for dummy arguments and parameters

! Assignment
first_name = 'Alice'   ! right-padded with spaces to len=20
last_name  = 'Smith'

! Concatenation
full_name = trim(first_name) // ' ' // trim(last_name)
! trim() removes trailing spaces

! Substring access
character(len=10) :: word = 'FORTRAN   '
write(*, *) word(1:7)   ! 'FORTRAN'
write(*, *) word(5:7)   ! 'RAN'

! Intrinsic functions
write(*, *) len('Hello')         ! 5 — declared length
write(*, *) len_trim('Hello   ') ! 5 — length without trailing spaces
write(*, *) index('FORTRAN', 'RAN')   ! 5 — position of substring
write(*, *) adjustl('  Hello')   ! 'Hello  ' — left adjust
write(*, *) adjustr('Hello  ')   ! '  Hello' — right adjust
write(*, *) repeat('ab', 3)      ! 'ababab'
write(*, *) scan('FORTRAN', 'RN')! 4 — position of first character in set

! Case conversion (Fortran has no built-in — use ichar/char)
! In practice, use a module or write a helper function
character :: c
c = 'a'
c = char(ichar(c) - 32)   ! 'A' — works for ASCII a-z only
```

---

## 9. Derived Types (Structs)

```fortran
module person_module
    implicit none

    type :: address_t
        character(len=50) :: street
        character(len=30) :: city
        character(len=10) :: postcode
    end type address_t

    type :: person_t
        character(len=30) :: name
        integer           :: age
        real              :: height
        type(address_t)   :: address    ! nested type
        logical           :: active = .true.  ! default value
    end type person_t

contains

    subroutine print_person(p)
        type(person_t), intent(in) :: p
        write(*, '(A, A)') 'Name:   ', trim(p%name)
        write(*, '(A, I3)') 'Age:    ', p%age
        write(*, '(A, A)') 'City:   ', trim(p%address%city)
    end subroutine print_person

end module person_module


program use_derived_type
    use person_module
    implicit none

    type(person_t) :: alice

    ! Structure constructor
    alice = person_t( &
        name    = 'Alice',    &
        age     = 30,         &
        height  = 1.75,       &
        address = address_t('123 Main St', 'Paris', '75001'), &
        active  = .true.      &
    )

    ! Member access with %
    write(*, *) alice%name
    write(*, *) alice%address%city
    alice%age = 31

    call print_person(alice)

    ! Array of derived type
    type(person_t) :: team(10)
    team(1)%name = 'Bob'
    team(1)%age  = 25

end program use_derived_type
```

---

## 10. Subprograms — Subroutines and Functions

### 10.1 Subroutines

```fortran
! Definition
subroutine swap(a, b)
    real, intent(inout) :: a, b
    real :: temp
    temp = a
    a    = b
    b    = temp
end subroutine swap

! Call
real :: x = 3.0, y = 7.0
call swap(x, y)
! x = 7.0, y = 3.0
```

### 10.2 Functions

```fortran
! Pure function — no side effects, preferred
pure function hypotenuse(a, b) result(h)
    real, intent(in) :: a, b
    real :: h
    h = sqrt(a**2 + b**2)
end function hypotenuse

! Elemental function — applies element-wise to arrays
elemental function celsius_to_fahrenheit(c) result(f)
    real, intent(in) :: c
    real :: f
    f = c * 9.0/5.0 + 32.0
end function celsius_to_fahrenheit

! Usage
real :: temps(5) = [0., 20., 37., 100., -40.]
real :: f_temps(5)
f_temps = celsius_to_fahrenheit(temps)   ! works on the whole array
```

### 10.3 Intent

`intent` declares how a dummy argument is used — the compiler enforces it.

| Intent | Meaning |
|---|---|
| `intent(in)` | Read-only — the subroutine cannot modify it |
| `intent(out)` | Write-only — must be defined before use; undefined on entry |
| `intent(inout)` | Read and write — value is passed in and a modified value passed back |
| *(none)* | Unrestricted — avoid; intent should always be specified |

```fortran
subroutine compute(a, b, result)
    real, intent(in)  :: a, b    ! inputs — cannot be modified
    real, intent(out) :: result  ! output — must be set before returning
    result = a + b
end subroutine compute
```

### 10.4 Optional arguments

```fortran
subroutine greet(name, times, prefix)
    character(len=*),           intent(in)  :: name
    integer,          optional, intent(in)  :: times
    character(len=*), optional, intent(in)  :: prefix

    integer           :: n
    character(len=20) :: pre

    n   = 1;        if (present(times))  n   = times
    pre = 'Hello';  if (present(prefix)) pre = prefix

    do i = 1, n
        write(*, '(A, A, A, A)') trim(pre), ', ', trim(name), '!'
    end do
end subroutine greet

! Calls
call greet('Alice')                         ! Hello, Alice!
call greet('Alice', times=3)                ! 3 times
call greet('Alice', prefix='Hi', times=2)  ! Hi, Alice! (twice)
```

### 10.5 Recursive procedures

```fortran
recursive function factorial(n) result(f)
    integer, intent(in) :: n
    integer(int64)      :: f
    if (n <= 1) then
        f = 1
    else
        f = n * factorial(n - 1)
    end if
end function factorial

! Fibonacci (tail-recursive style)
recursive function fib(n) result(f)
    integer, intent(in) :: n
    integer :: f
    select case (n)
        case (0)    ; f = 0
        case (1)    ; f = 1
        case default; f = fib(n-1) + fib(n-2)
    end select
end function fib
```

### 10.6 Internal subprograms (CONTAINS)

```fortran
program main
    implicit none
    real :: x = 2.5, y = 3.0

    write(*, *) area_rectangle(x, y)
    call normalize(x)
    write(*, *) x

contains

    pure function area_rectangle(w, h) result(a)
        real, intent(in) :: w, h
        real :: a
        a = w * h
    end function area_rectangle

    subroutine normalize(v)
        real, intent(inout) :: v
        if (abs(v) > 1.0e-10) v = v / abs(v)
    end subroutine normalize

end program main
```

---

## 11. Modules

Modules are the standard way to share code, types, and data between program units.

```fortran
! math_constants.f90
module math_constants
    use, intrinsic :: iso_fortran_env, only: real64
    implicit none

    ! Everything public by default — restrict with private/public
    private   ! make everything private by default
    public :: PI, E, GOLDEN_RATIO, dp

    integer, parameter :: dp = real64
    real(dp), parameter :: PI           = 3.14159265358979323846_dp
    real(dp), parameter :: E            = 2.71828182845904523536_dp
    real(dp), parameter :: GOLDEN_RATIO = 1.61803398874989484820_dp

end module math_constants


! geometry.f90
module geometry
    use math_constants
    implicit none
    private
    public :: circle_area, sphere_volume, point_t

    type :: point_t
        real(dp) :: x, y
    end type point_t

contains

    pure function circle_area(r) result(a)
        real(dp), intent(in) :: r
        real(dp) :: a
        a = PI * r**2
    end function circle_area

    pure function sphere_volume(r) result(v)
        real(dp), intent(in) :: r
        real(dp) :: v
        v = (4.0_dp / 3.0_dp) * PI * r**3
    end function sphere_volume

    pure function distance(p1, p2) result(d)
        type(point_t), intent(in) :: p1, p2
        real(dp) :: d
        d = sqrt((p2%x - p1%x)**2 + (p2%y - p1%y)**2)
    end function distance

end module geometry


! main.f90
program use_modules
    use geometry
    use math_constants, only: PI, dp   ! import only what you need
    implicit none

    write(*, '(A, F10.4)') 'Circle area (r=5):   ', circle_area(5.0_dp)
    write(*, '(A, F10.4)') 'Sphere volume (r=5): ', sphere_volume(5.0_dp)
    write(*, '(A, F10.6)') 'PI = ', PI

end program use_modules
```

**Compile modules in dependency order:**
```bash
gfortran -c math_constants.f90   # produces math_constants.mod + math_constants.o
gfortran -c geometry.f90          # uses math_constants.mod
gfortran -c main.f90
gfortran -o program main.o geometry.o math_constants.o
```

---

## 12. Pointers

Fortran pointers associate a pointer variable with a target (another variable or an array section).

```fortran
program pointers
    implicit none

    real, target  :: x = 3.14     ! TARGET attribute — can be pointed to
    real, pointer :: ptr => null() ! POINTER — starts as null

    ! Associate
    ptr => x
    write(*, *) ptr   ! 3.14

    ! Modify through pointer
    ptr = 42.0
    write(*, *) x     ! 42.0 — x was modified

    ! Disassociate
    nullify(ptr)

    ! Check association status
    if (.not. associated(ptr)) write(*, *) 'ptr is null'

    ! Dynamic allocation via pointer
    real, pointer :: dynamic(:)
    allocate(dynamic(10))
    dynamic = 0.0
    dynamic(1) = 99.0
    deallocate(dynamic)

    ! Pointer to array section
    real, target  :: matrix(5,5)
    real, pointer :: column(:)
    column => matrix(:, 3)   ! column points to column 3 of matrix

end program pointers
```

> Use allocatable arrays over pointers whenever possible. Pointers require manual management and can produce memory leaks. Use pointers specifically for linked data structures or when aliasing is intentional.

---

## 13. Input / Output

### 13.1 Formatted I/O

```fortran
! write(unit, format) items
write(*, *)          x, y, z         ! list-directed (free format) to stdout
write(*, '(F8.2)')   x               ! formatted to stdout
write(10, '(A, F8.2)') 'Value: ', x ! formatted to unit 10 (a file)

! read(unit, format) items
read(*, *)          x
read(*, '(F8.2)')   x
read(10, '(A20)')   name

! Named format statement (classic style)
100 format(I6, 2X, F8.2, 2X, A20)
write(*, 100) n, x, label

! Internal read/write (string conversion)
character(len=20) :: str
write(str, '(F8.2)') 3.14     ! str = '    3.14'
read(str, '(F8.2)') x         ! x  = 3.14
```

### 13.2 Format descriptors

| Descriptor | Meaning | Example |
|---|---|---|
| `Iw` | Integer, width w | `I6` → `   -42` |
| `Fw.d` | Fixed float, w wide, d decimals | `F8.2` → `    3.14` |
| `Ew.d` | Scientific notation | `E12.4` → ` 3.1416E+00` |
| `ESw.d` | Engineering notation (exponent mult. of 3) | `ES12.4` |
| `Gw.d` | General (F or E, whichever fits) | `G12.4` |
| `A` | Character string | `A` or `A20` |
| `Lw` | Logical | `L2` → ` T` or ` F` |
| `nX` | Skip n spaces | `2X` |
| `/` | New line | `'(A, /, A)'` |
| `\` | No newline (GFortran extension) | |
| `nH...` | Hollerith (old — avoid) | |
| `Tc` | Tab to column c | `T20` |
| `(n)` | Repeat: `3(F8.2, 1X)` | |
| `*` | Unlimited repeat | `*(F8.2)` |

```fortran
! Practical examples
write(*, '(A30, I6)')    'Count:              ', n
write(*, '(A30, F10.4)') 'Value:              ', x
write(*, '(A30, ES12.4)')'Scientific:         ', big_number
write(*, '(3(F8.2, 2X))') a, b, c          ! three floats with spaces
write(*, '(I0)')         n                 ! integer without leading spaces
write(*, '(*(G0, " "))') array             ! all elements, separated by spaces

! advance='no' — no newline after write
write(*, '(A)', advance='no') 'Enter value: '
read(*, *) x
```

### 13.3 File I/O

```fortran
integer :: unit_num, io_status
character(len=100) :: line

! OPEN a file
open(newunit=unit_num,       & ! newunit: compiler picks an available unit number
     file='data.txt',        &
     status='old',           & ! 'old'=must exist, 'new'=must not exist, 'replace', 'scratch', 'unknown'
     action='read',          & ! 'read', 'write', 'readwrite'
     form='formatted',       & ! 'formatted'(text) or 'unformatted'(binary)
     iostat=io_status)

if (io_status /= 0) then
    write(*, '(A, I0)') 'Open failed, iostat = ', io_status
    stop 1
end if

! READ until end of file
do
    read(unit_num, '(A)', iostat=io_status) line
    if (io_status < 0) exit    ! end of file
    if (io_status > 0) then    ! read error
        write(*, *) 'Read error'
        exit
    end if
    write(*, *) trim(line)
end do

! CLOSE
close(unit_num)

! WRITE to file
open(newunit=unit_num, file='output.txt', status='replace', action='write')
write(unit_num, '(A, F10.4)') 'Result: ', result
close(unit_num)

! BACKSPACE — move back one record
backspace(unit_num)

! REWIND — go to beginning
rewind(unit_num)

! INQUIRE — query file properties
logical :: file_exists
inquire(file='data.txt', exist=file_exists)
if (.not. file_exists) write(*, *) 'File not found'

integer :: record_length
inquire(unit=unit_num, recl=record_length)
```

---

## 14. Intrinsic Functions

### Mathematical

```fortran
! Trigonometric (arguments in radians)
sin(x)    cos(x)    tan(x)
asin(x)   acos(x)   atan(x)
atan2(y, x)           ! four-quadrant arctangent

! Hyperbolic
sinh(x)   cosh(x)   tanh(x)

! Exponential and logarithmic
exp(x)    log(x)    log10(x)    sqrt(x)

! Rounding
floor(x)    ! largest integer <= x
ceiling(x)  ! smallest integer >= x
nint(x)     ! nearest integer (rounds half away from zero)
int(x)      ! truncation toward zero
aint(x)     ! truncation — result is real
anint(x)    ! nearest integer — result is real

! Absolute value / sign
abs(x)
sign(a, b)  ! returns abs(a) * sign(b)

! Max / min
max(a, b, c, ...)
min(a, b, c, ...)
```

### Array intrinsics

```fortran
! Shape and size
size(a)         ! total number of elements
size(a, dim=1)  ! number of elements along dimension 1
shape(a)        ! integer array of dimensions
rank(a)         ! number of dimensions (Fortran 2018)
lbound(a)       ! lower bounds
ubound(a)       ! upper bounds

! Reduction
sum(a)          sum(a, dim=1)        sum(a, mask=(a>0))
product(a)      maxval(a)            minval(a)
maxloc(a)       minloc(a)
any(a > 0)      all(a > 0)          count(a > 0)

! Construction
reshape(source, shape)
pack(array, mask)       ! extract elements where mask is true → 1D array
unpack(vector, mask, field)
spread(source, dim, ncopies)
merge(tsource, fsource, mask)  ! element-wise ternary

! Matrix
matmul(a, b)
dot_product(a, b)
transpose(a)

! Sorting (Fortran has no built-in sort — use a library or write one)
```

### Bit manipulation

```fortran
iand(a, b)      ior(a, b)       ieor(a, b)    ! AND, OR, XOR
not(a)
ishft(a, shift)   ! logical shift
ishftc(a, shift)  ! circular shift
ibset(a, pos)     ! set bit
ibclr(a, pos)     ! clear bit
btest(a, pos)     ! test bit → logical
```

### Numeric inquiry

```fortran
huge(x)       ! largest positive number of same type and kind
tiny(x)       ! smallest positive number
epsilon(x)    ! smallest x such that 1 + x /= 1
digits(x)     ! number of significant binary digits
precision(x)  ! decimal precision
range(x)      ! decimal exponent range
kind(x)       ! kind parameter
```

### Type conversion

```fortran
real(n)       ! integer to real
int(x)        ! real to integer (truncates)
dble(x)       ! to double precision
cmplx(x, y)  ! to complex
char(n)       ! integer to character (ASCII)
ichar(c)      ! character to integer (ASCII)
```

---

## 15. Preprocessing and Compiler Directives

### Conditional compilation (with cpp preprocessor)

Compile with `gfortran -cpp source.f90` to enable the C preprocessor.

```fortran
#define DEBUG
#define MAX_SIZE 1000
#define DOUBLE_PRECISION

#ifdef DEBUG
    write(*, *) 'Debug: x = ', x
#endif

#ifndef DOUBLE_PRECISION
    real :: x
#else
    real(8) :: x
#endif

#if MAX_SIZE > 500
    write(*, *) 'Large table mode'
#endif
```

### GFortran-specific directives

```fortran
!$omp parallel do          ! OpenMP (parallel loops)
do i = 1, n
    a(i) = b(i) + c(i)
end do
!$omp end parallel do

! Enable with: gfortran -fopenmp source.f90
```

### Common gfortran flags

| Flag | Meaning |
|---|---|
| `-g` | Debug symbols |
| `-O0` / `-O2` / `-O3` | Optimization level |
| `-Wall -Wextra` | All warnings |
| `-fcheck=all` | Runtime checks (bounds, null pointers, etc.) |
| `-ffpe-trap=invalid,zero,overflow` | Trap floating-point exceptions |
| `-fbacktrace` | Print call stack on error |
| `-cpp` | Enable C preprocessor |
| `-fopenmp` | Enable OpenMP parallelism |
| `-march=native` | Optimize for current CPU |
| `-static` | Static linking |
| `-c` | Compile to object file only |
| `-I<dir>` | Add module search path |
| `-J<dir>` | Place .mod files in this directory |

---

## 16. Error Handling

### IOSTAT — file and read/write errors

```fortran
integer :: ios
character(len=100) :: err_msg

open(newunit=u, file='data.txt', iostat=ios, iomsg=err_msg)
if (ios /= 0) then
    write(*, '(A, A)') 'Open error: ', trim(err_msg)
    stop 1
end if

read(u, *, iostat=ios) x
if (ios < 0) write(*, *) 'End of file'
if (ios > 0) write(*, *) 'Read error'
```

### STAT — allocation errors

```fortran
integer :: alloc_stat
real, allocatable :: a(:)

allocate(a(n), stat=alloc_stat)
if (alloc_stat /= 0) then
    write(*, '(A, I0)') 'Allocation failed, stat = ', alloc_stat
    stop 1
end if

deallocate(a, stat=alloc_stat)
```

### ERROR STOP (Fortran 2008+)

```fortran
if (denominator == 0.0) then
    error stop 'Division by zero — cannot continue'
end if

error stop 42   ! stop with exit code
```

### STOP with message

```fortran
if (n < 0) stop 'Error: n must be non-negative'
if (error_flag) stop 1   ! exit code 1
```

### Floating-point exception trapping

```fortran
! Compile with: gfortran -ffpe-trap=invalid,zero,overflow source.f90
! The program will abort with a traceback on:
!   - invalid operations (sqrt of negative, 0/0)
!   - division by zero
!   - overflow
```

---

## 17. Quick Reference Card

### Type declarations at a glance

```fortran
use, intrinsic :: iso_fortran_env, only: int32, int64, real32, real64
implicit none

integer(int32)          :: i          ! 32-bit integer
integer(int64)          :: big        ! 64-bit integer
real(real32)            :: x          ! single precision
real(real64)            :: d          ! double precision
complex(real64)         :: z          ! double precision complex
logical                 :: flag = .false.
character(len=50)       :: s
character(len=*), parameter :: msg = "hello"   ! length from initializer
```

### Loop patterns

```fortran
do i = start, stop [, step]   ! counted
do while (condition)           ! conditional
do                             ! infinite — use EXIT to leave
end do
```

### Array creation idioms

```fortran
! Array constructor
a = [1, 2, 3, 4, 5]
a = [(i, i = 1, 10)]                   ! implied DO: 1..10
a = [(i**2, i = 1, 10)]               ! squares: 1, 4, 9, ..., 100
a = [(real(i)*0.1, i = 0, 9)]        ! 0.0, 0.1, ..., 0.9
```

### Frequently confused things

| | |
|---|---|
| `real :: x = 1.0` | Initialization (also implies `SAVE` in subprograms!) |
| `x = 1.0` | Assignment in executable section |
| `intent(in)` | Dummy arg cannot be modified |
| `intent(out)` | Dummy arg must be set, undefined on entry |
| `intent(inout)` | Dummy arg can be read and written |
| `pure function` | No side effects — can be called from `do concurrent` |
| `elemental function` | Pure + applies element-wise to arrays |
| `allocatable` | Automatic deallocation; preferred over pointers |
| `target` | Required before a variable can be pointed to |
| `associated(ptr)` | Check if pointer is associated (not null) |
| `allocated(arr)` | Check if allocatable array is allocated |
| `//` | String concatenation (`"a" // "b"` = `"ab"`) |
| `**` | Exponentiation (`2**10` = `1024`) |
| `==` and `/=` | Equal and not-equal (also `.eq.` and `.ne.`) |

---

---

## 🇫🇷 Français

### Table des matières

1. [Format source](#1-format-source-1)
2. [Structure d'un programme](#2-structure-dun-programme)
3. [Types de données](#3-types-de-données)
4. [Variables — déclaration et attributs](#4-variables--déclaration-et-attributs)
5. [Opérateurs](#5-opérateurs-1)
6. [Structures de contrôle](#6-structures-de-contrôle)
7. [Tableaux](#7-tableaux)
8. [Chaînes de caractères](#8-chaînes-de-caractères)
9. [Types dérivés (structures)](#9-types-dérivés-structures)
10. [Sous-programmes](#10-sous-programmes)
11. [Modules](#11-modules-1)
12. [Pointeurs](#12-pointeurs-1)
13. [Entrées / Sorties](#13-entrées--sorties)
14. [Fonctions intrinsèques](#14-fonctions-intrinsèques-1)
15. [Compilation et options](#15-compilation-et-options)
16. [Gestion des erreurs](#16-gestion-des-erreurs-1)
17. [Carte de référence rapide](#17-carte-de-référence-rapide-1)

---

## 1. Format source

### Format fixe (Fortran 77 et antérieur)

```
Col 1     : 'C' ou '*' = commentaire
Col 1–5   : Étiquette de ligne
Col 6     : Marqueur de continuation (tout sauf '0' ou ' ')
Col 7–72  : Instruction
```

Fichiers avec extension `.f` ou `.for`.

### Format libre (Fortran 90+ — à utiliser)

- Aucune restriction de colonne
- `!` commence un commentaire
- `&` en fin de ligne = continuation sur la ligne suivante
- Plusieurs instructions par ligne séparées par `;`
- Fichiers avec extension `.f90`, `.f95`, `.f03`, `.f08`

```fortran
! Commentaire en format libre
x = 1.0 + &    ! continuation
    2.0

a = 1; b = 2; c = 3
```

---

## 2. Structure d'un programme

```fortran
program nom_du_programme
    implicit none           ! TOUJOURS
    ! déclarations
    ! instructions exécutables
contains
    ! sous-programmes internes (optionnel)
end program nom_du_programme
```

---

## 3. Types de données

### Types intrinsèques

| Type | Exemple | Description |
|---|---|---|
| `INTEGER` | `integer :: n` | Entiers |
| `REAL` | `real :: x` | Virgule flottante (simple précision par défaut) |
| `DOUBLE PRECISION` | `double precision :: d` | Flottant 64 bits (syntaxe legacy) |
| `COMPLEX` | `complex :: z` | Nombre complexe |
| `LOGICAL` | `logical :: flag` | Booléen : `.true.` ou `.false.` |
| `CHARACTER` | `character(len=50) :: nom` | Chaîne de longueur fixe |

### KIND — contrôle de la précision

```fortran
use, intrinsic :: iso_fortran_env, only: &
    int32, int64, real32, real64, real128

integer(int32)  :: entier_standard     ! exactement 32 bits
integer(int64)  :: grand_entier        ! exactement 64 bits
real(real32)    :: simple              ! IEEE 754 32 bits
real(real64)    :: double              ! IEEE 754 64 bits

! Suffixe de kind sur les littéraux — OBLIGATOIRE pour la précision
real(real64) :: x = 1.0_real64        ! correct
real(real64) :: y = 1.0               ! ATTENTION : 1.0 est single par défaut
```

> Suffixe toujours les littéraux avec leur kind : `1.0_dp` et non `1.0`. Sans suffixe, un littéral est du kind par défaut quelle que soit la variable — source courante de perte de précision.

### Constantes (PARAMETER)

```fortran
integer,         parameter :: TAILLE_MAX = 1000
real(real64),    parameter :: PI = 3.14159265358979323846_real64
character(len=*),parameter :: VERSION = "1.0.0"
```

### Implicit none

```fortran
implicit none
```

**Utilise-le toujours.** Sans lui, Fortran applique le typage implicite : variables commençant par `i`–`n` sont `INTEGER`, le reste est `REAL`. Cette règle a causé des bugs célèbres dans du code scientifique. `implicit none` oblige à déclarer explicitement chaque variable.

---

## 4. Variables — déclaration et attributs

```fortran
integer :: n
real(real64) :: x, y, z

! Avec initialisation
integer :: compteur = 0
real    :: pi = 3.14159265

! SAVE — conserve la valeur entre les appels (uniquement dans les sous-programmes)
integer, save :: nb_appels = 0

! ALLOCATABLE — mémoire dynamique
real, allocatable :: donnees(:)

! POINTER et TARGET
real, pointer :: ptr => null()
real, target  :: valeur = 42.0
```

---

## 5. Opérateurs

### Arithmétiques

| Opérateur | Signification |
|---|---|
| `+` `-` `*` `/` | Addition, soustraction, multiplication, division |
| `**` | Exponentiation |

> Division entière : `7 / 2 = 3`. Utilise `7.0 / 2` ou `real(7) / 2` pour `3.5`.

### Comparaison

| Opérateur | Symbolique |
|---|---|
| `==` | `.EQ.` |
| `/=` | `.NE.` |
| `<` `<=` `>` `>=` | `.LT.` `.LE.` `.GT.` `.GE.` |

### Logiques

`.and.`  `.or.`  `.not.`  `.eqv.`  `.neqv.`

### Chaînes

`//` — concaténation : `"Bon" // "jour"` = `"Bonjour"`

---

## 6. Structures de contrôle

### IF / ELSE IF / ELSE

```fortran
if (x > 0.0) then
    write(*, *) 'Positif'
else if (x < 0.0) then
    write(*, *) 'Négatif'
else
    write(*, *) 'Zéro'
end if
```

### SELECT CASE

```fortran
select case (jour)
    case (1)    ; write(*, *) 'Lundi'
    case (2)    ; write(*, *) 'Mardi'
    case (3:5)  ; write(*, *) 'Mercredi à vendredi'
    case (6, 7) ; write(*, *) 'Week-end'
    case default; write(*, *) 'Invalide'
end select
```

### Boucles DO

```fortran
! Boucle comptée
do i = 1, 10
    write(*, *) i
end do

! Avec pas
do i = 1, 100, 5    ! 1, 6, 11, ..., 96
    write(*, *) i
end do

! DO WHILE
do while (x > 1.0e-6)
    x = x / 2.0
end do

! Boucle infinie — sortie avec EXIT
do
    read(*, *) saisie
    if (saisie == 0) exit
    call traiter(saisie)
end do

! Boucles nommées — EXIT et CYCLE ciblés
externe: do i = 1, n
    interne: do j = 1, m
        if (matrice(i,j) == 0) cycle externe
        if (matrice(i,j) < 0) exit externe
    end do interne
end do externe
```

### EXIT et CYCLE

```fortran
do i = 1, 10
    if (mod(i, 2) == 0) cycle   ! sauter les pairs
    if (i > 7) exit              ! sortir si > 7
    write(*, *) i
end do
```

---

## 7. Tableaux

### Déclaration

```fortran
integer :: comptages(10)           ! indices 1 à 10
real    :: vecteur(0:99)           ! indices 0 à 99
real    :: matrice(3, 3)           ! 3×3
real    :: tenseur(4, 4, 4)        ! rang 3

integer :: premiers(5) = [2, 3, 5, 7, 11]
```

### Opérations sur tableaux — la force de Fortran

```fortran
real :: a(5) = [1., 2., 3., 4., 5.]
real :: b(5) = [10., 20., 30., 40., 50.]
real :: c(5)

c = a + b           ! [11, 22, 33, 44, 55] — sans boucle
c = a * b           ! multiplication élément par élément
c = sqrt(a)         ! racine carrée de chaque élément

write(*, *) sum(a)       ! 15.0
write(*, *) maxval(a)    ! 5.0
write(*, *) any(a > 3)   ! .true.
write(*, *) all(a > 0)   ! .true.
write(*, *) count(a > 2) ! 3

! Multiplication matricielle
real :: m(3,3), n_mat(3,3), res(3,3)
res = matmul(m, n_mat)

! WHERE — affectation conditionnelle
where (a > 0.0)
    a = sqrt(a)
elsewhere
    a = 0.0
end where
```

### Tableaux allocatables

```fortran
real, allocatable :: donnees(:)
real, allocatable :: grille(:,:)
integer :: n = 100, stat_alloc

allocate(donnees(n), stat=stat_alloc)
if (stat_alloc /= 0) then
    write(*, *) 'Échec d''allocation'
    stop 1
end if

donnees = 0.0
if (allocated(donnees)) write(*, *) 'Taille :', size(donnees)

deallocate(donnees)
```

### Sections de tableaux (slicing)

```fortran
real :: a(10)
a = [(real(i), i = 1, 10)]   ! [1, 2, ..., 10]

a(3:7)      ! éléments 3, 4, 5, 6, 7
a(1:9:2)    ! éléments 1, 3, 5, 7, 9
a(10:1:-1)  ! renversé

real :: m(5,5)
m(:, 1)     ! colonne entière 1
m(1, :)     ! ligne entière 1
m(2:4, 2:4) ! sous-matrice 3×3
```

---

## 8. Chaînes de caractères

```fortran
character(len=20) :: prenom, nom_famille
character(len=41) :: nom_complet

prenom      = 'Alice'
nom_famille = 'Dupont'
nom_complet = trim(prenom) // ' ' // trim(nom_famille)

! Sous-chaîne
character(len=10) :: mot = 'FORTRAN   '
write(*, *) mot(1:7)    ! 'FORTRAN'

! Fonctions intrinsèques
write(*, *) len('Bonjour')          ! longueur déclarée
write(*, *) len_trim('Bonjour   ')  ! longueur sans espaces finaux
write(*, *) index('FORTRAN', 'RAN') ! 5 — position de la sous-chaîne
write(*, *) adjustl('  Bonjour')    ! 'Bonjour  '
write(*, *) adjustr('Bonjour  ')    ! '  Bonjour'
write(*, *) repeat('ab', 3)         ! 'ababab'
```

---

## 9. Types dérivés (structures)

```fortran
module types_module
    implicit none

    type :: adresse_t
        character(len=50) :: rue
        character(len=30) :: ville
        character(len=10) :: code_postal
    end type adresse_t

    type :: personne_t
        character(len=30) :: nom
        integer           :: age
        real              :: taille
        type(adresse_t)   :: adresse
        logical           :: actif = .true.
    end type personne_t

contains

    subroutine afficher_personne(p)
        type(personne_t), intent(in) :: p
        write(*, '(A, A)') 'Nom  : ', trim(p%nom)
        write(*, '(A, I3)') 'Âge  : ', p%age
        write(*, '(A, A)') 'Ville: ', trim(p%adresse%ville)
    end subroutine afficher_personne

end module types_module


program utiliser_types
    use types_module
    implicit none

    type(personne_t) :: alice

    ! Constructeur de structure
    alice = personne_t( &
        nom     = 'Alice',    &
        age     = 30,         &
        taille  = 1.75,       &
        adresse = adresse_t('12 rue de la Paix', 'Paris', '75001'), &
        actif   = .true.      &
    )

    ! Accès avec %
    write(*, *) alice%nom
    write(*, *) alice%adresse%ville
    alice%age = 31

    call afficher_personne(alice)

end program utiliser_types
```

---

## 10. Sous-programmes

### Sous-routines

```fortran
subroutine echanger(a, b)
    real, intent(inout) :: a, b
    real :: temp
    temp = a; a = b; b = temp
end subroutine echanger

! Appel
real :: x = 3.0, y = 7.0
call echanger(x, y)
```

### Fonctions

```fortran
pure function hypothenuse(a, b) result(h)
    real, intent(in) :: a, b
    real :: h
    h = sqrt(a**2 + b**2)
end function hypothenuse

elemental function celsius_en_fahrenheit(c) result(f)
    real, intent(in) :: c
    real :: f
    f = c * 9.0/5.0 + 32.0
end function celsius_en_fahrenheit

! Fonctionne sur un tableau entier
real :: temperatures(5) = [0., 20., 37., 100., -40.]
real :: en_fahrenheit(5)
en_fahrenheit = celsius_en_fahrenheit(temperatures)
```

### Intent

| Intent | Signification |
|---|---|
| `intent(in)` | Lecture seule — le sous-programme ne peut pas le modifier |
| `intent(out)` | Écriture seule — doit être défini avant retour |
| `intent(inout)` | Lecture et écriture |

### Arguments optionnels

```fortran
subroutine saluer(nom, fois, prefixe)
    character(len=*),           intent(in) :: nom
    integer,          optional, intent(in) :: fois
    character(len=*), optional, intent(in) :: prefixe
    integer :: n
    n = 1; if (present(fois)) n = fois
    ! ...
end subroutine saluer

call saluer('Alice')
call saluer('Alice', fois=3)
call saluer('Alice', prefixe='Bonjour', fois=2)
```

### Fonctions récursives

```fortran
recursive function factorielle(n) result(f)
    integer, intent(in) :: n
    integer(int64)      :: f
    if (n <= 1) then
        f = 1
    else
        f = n * factorielle(n - 1)
    end if
end function factorielle
```

### Sous-programmes internes (CONTAINS)

```fortran
program principal
    implicit none
    real :: x = 2.5, y = 3.0
    write(*, *) aire_rectangle(x, y)
contains
    pure function aire_rectangle(l, h) result(a)
        real, intent(in) :: l, h
        real :: a
        a = l * h
    end function aire_rectangle
end program principal
```

---

## 11. Modules

```fortran
! constantes_math.f90
module constantes_math
    use, intrinsic :: iso_fortran_env, only: real64
    implicit none
    private
    public :: PI, E, dp

    integer, parameter :: dp = real64
    real(dp), parameter :: PI = 3.14159265358979323846_dp
    real(dp), parameter :: E  = 2.71828182845904523536_dp

end module constantes_math


! geometrie.f90
module geometrie
    use constantes_math
    implicit none
    private
    public :: aire_cercle, volume_sphere

contains

    pure function aire_cercle(r) result(a)
        real(dp), intent(in) :: r
        real(dp) :: a
        a = PI * r**2
    end function aire_cercle

    pure function volume_sphere(r) result(v)
        real(dp), intent(in) :: r
        real(dp) :: v
        v = (4.0_dp / 3.0_dp) * PI * r**3
    end function volume_sphere

end module geometrie


! principal.f90
program utiliser_modules
    use geometrie
    use constantes_math, only: PI, dp
    implicit none

    write(*, '(A, F10.4)') 'Aire cercle (r=5)    : ', aire_cercle(5.0_dp)
    write(*, '(A, F10.4)') 'Volume sphère (r=5)  : ', volume_sphere(5.0_dp)

end program utiliser_modules
```

**Compilation dans l'ordre des dépendances :**
```bash
gfortran -c constantes_math.f90
gfortran -c geometrie.f90
gfortran -c principal.f90
gfortran -o programme principal.o geometrie.o constantes_math.o
```

---

## 12. Pointeurs

```fortran
real, target  :: x = 3.14
real, pointer :: ptr => null()

ptr => x              ! association
write(*, *) ptr       ! 3.14
ptr = 42.0
write(*, *) x         ! 42.0 — x a été modifié

nullify(ptr)          ! désassociation
if (.not. associated(ptr)) write(*, *) 'ptr est nul'

! Allocation dynamique via pointeur
real, pointer :: dynamique(:)
allocate(dynamique(10))
dynamique = 0.0
deallocate(dynamique)

! Pointeur vers une section de tableau
real, target  :: matrice(5,5)
real, pointer :: colonne(:)
colonne => matrice(:, 3)   ! colonne pointe vers la colonne 3
```

> Préfère les tableaux `allocatable` aux pointeurs. Les pointeurs nécessitent une gestion manuelle et peuvent produire des fuites mémoire. Utilise les pointeurs spécifiquement pour les structures de données chaînées ou quand l'alias est intentionnel.

---

## 13. Entrées / Sorties

### I/O formatées

```fortran
write(*, *) x, y, z                      ! format libre vers stdout
write(*, '(F8.2)') x                     ! format explicite
write(*, '(A30, F10.4)') 'Valeur : ', x
write(*, '(I0)') n                       ! entier sans espaces inutiles
write(*, '(3(F8.2, 2X))') a, b, c        ! trois flottants avec espaces

read(*, *) x                             ! lecture format libre
write(*, '(A)', advance='no') 'Saisir : '
read(*, '(A)') saisie                    ! sans saut de ligne après le prompt
```

### Descripteurs de format

| Descripteur | Signification |
|---|---|
| `Iw` | Entier, largeur w |
| `Fw.d` | Flottant fixe, w caractères, d décimales |
| `Ew.d` | Notation scientifique |
| `ESw.d` | Notation ingénierie (exposant multiple de 3) |
| `A` | Chaîne de caractères |
| `Lw` | Logique |
| `nX` | n espaces |
| `/` | Nouvelle ligne |
| `Tc` | Tabulation à la colonne c |

### I/O de fichiers

```fortran
integer :: uf, ios
character(len=100) :: ligne

open(newunit=uf, file='donnees.txt', status='old', &
     action='read', iostat=ios)
if (ios /= 0) then
    write(*, '(A, I0)') 'Erreur ouverture, ios = ', ios
    stop 1
end if

do
    read(uf, '(A)', iostat=ios) ligne
    if (ios < 0) exit    ! fin de fichier
    if (ios > 0) then    ! erreur de lecture
        write(*, *) 'Erreur de lecture'
        exit
    end if
    write(*, *) trim(ligne)
end do

close(uf)

! Écriture
open(newunit=uf, file='sortie.txt', status='replace', action='write')
write(uf, '(A, F10.4)') 'Résultat : ', resultat
close(uf)

! Vérifier l'existence d'un fichier
logical :: existe
inquire(file='donnees.txt', exist=existe)
if (.not. existe) write(*, *) 'Fichier introuvable'
```

---

## 14. Fonctions intrinsèques

### Mathématiques

```fortran
sin(x)  cos(x)  tan(x)  asin(x)  acos(x)  atan(x)  atan2(y,x)
sinh(x) cosh(x) tanh(x)
exp(x)  log(x)  log10(x)  sqrt(x)
abs(x)  sign(a, b)
floor(x)    ! plus grand entier <= x
ceiling(x)  ! plus petit entier >= x
nint(x)     ! entier le plus proche
max(a,b,c)  min(a,b,c)
```

### Tableaux

```fortran
size(a)           ! nombre total d'éléments
size(a, dim=1)    ! taille selon la dimension 1
shape(a)          ! tableau des dimensions
lbound(a)         ! bornes inférieures
ubound(a)         ! bornes supérieures

sum(a)  product(a)  maxval(a)  minval(a)
maxloc(a)  minloc(a)
any(a > 0)  all(a > 0)  count(a > 0)

matmul(a, b)     ! multiplication matricielle
dot_product(a,b) ! produit scalaire
transpose(a)     ! transposée
reshape(src, sh) ! reformater
```

### Requêtes numériques

```fortran
huge(x)       ! plus grand nombre positif du même type
tiny(x)       ! plus petit nombre positif normalisé
epsilon(x)    ! plus petit x tel que 1 + x /= 1
precision(x)  ! précision décimale
kind(x)       ! paramètre kind
```

### Conversion de types

```fortran
real(n)       ! entier → réel
int(x)        ! réel → entier (troncature)
dble(x)       ! → double précision
char(n)       ! entier → caractère ASCII
ichar(c)      ! caractère → entier ASCII
```

---

## 15. Compilation et options

### Options gfortran courantes

| Option | Signification |
|---|---|
| `-g` | Symboles de débogage |
| `-O0` / `-O2` / `-O3` | Niveau d'optimisation |
| `-Wall -Wextra` | Tous les avertissements |
| `-fcheck=all` | Vérifications runtime (bornes, pointeurs, etc.) |
| `-ffpe-trap=invalid,zero,overflow` | Intercepter les exceptions virgule flottante |
| `-fbacktrace` | Afficher la pile d'appel en cas d'erreur |
| `-cpp` | Activer le préprocesseur C |
| `-fopenmp` | Activer le parallélisme OpenMP |
| `-march=native` | Optimiser pour le CPU courant |
| `-c` | Compiler en fichier objet uniquement |
| `-J<rép>` | Placer les fichiers `.mod` dans ce répertoire |

---

## 16. Gestion des erreurs

```fortran
! IOSTAT — erreurs fichier / lecture
integer :: ios
character(len=256) :: msg_erreur

open(newunit=u, file='donnees.txt', iostat=ios, iomsg=msg_erreur)
if (ios /= 0) then
    write(*, '(A, A)') 'Erreur : ', trim(msg_erreur)
    stop 1
end if

read(u, *, iostat=ios) x
if (ios < 0) write(*, *) 'Fin de fichier'
if (ios > 0) write(*, *) 'Erreur de lecture'

! STAT — erreurs d'allocation
integer :: stat_alloc
real, allocatable :: a(:)

allocate(a(n), stat=stat_alloc)
if (stat_alloc /= 0) then
    write(*, '(A, I0)') 'Échec allocation, stat = ', stat_alloc
    stop 1
end if

! ERROR STOP (Fortran 2008+)
if (denominateur == 0.0) then
    error stop 'Division par zéro — impossible de continuer'
end if

! Intercepter les exceptions virgule flottante
! Compiler avec : gfortran -ffpe-trap=invalid,zero,overflow source.f90
! Le programme s'arrêtera avec une trace d'appel sur :
!   - opérations invalides (sqrt négatif, 0/0)
!   - division par zéro
!   - dépassement de capacité
```

---

## 17. Carte de référence rapide

### Déclarations en un coup d'œil

```fortran
use, intrinsic :: iso_fortran_env, only: int32, int64, real32, real64
implicit none

integer(int32)            :: i
integer(int64)            :: grand
real(real32)              :: x
real(real64)              :: d
complex(real64)           :: z
logical                   :: flag = .false.
character(len=50)         :: s
character(len=*), parameter :: msg = "bonjour"
```

### Constructeurs de tableaux

```fortran
a = [1, 2, 3, 4, 5]
a = [(i, i = 1, 10)]              ! DO implicite : 1..10
a = [(i**2, i = 1, 10)]           ! carrés : 1, 4, 9, ..., 100
a = [(real(i)*0.1, i = 0, 9)]    ! 0.0, 0.1, ..., 0.9
```

### Ce qu'on confond souvent

| | |
|---|---|
| `real :: x = 1.0` | Initialisation (implique `SAVE` dans les sous-programmes !) |
| `x = 1.0` | Affectation dans la section exécutable |
| `intent(in)` | Argument muet non modifiable |
| `intent(out)` | Doit être défini avant retour, indéfini à l'entrée |
| `intent(inout)` | Peut être lu et écrit |
| `pure function` | Pas d'effets de bord — utilisable avec `do concurrent` |
| `elemental function` | Pure + s'applique élément par élément aux tableaux |
| `allocatable` | Désallocation automatique — préférable aux pointeurs |
| `target` | Requis avant qu'une variable puisse être pointée |
| `associated(ptr)` | Vérifie si le pointeur est associé (non nul) |
| `allocated(arr)` | Vérifie si le tableau allocatable est alloué |
| `//` | Concaténation de chaînes |
| `**` | Exponentiation (`2**10` = `1024`) |
| `==` et `/=` | Égal et différent (aussi `.eq.` et `.ne.`) |
