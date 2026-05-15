# FORTRAN — FORmula TRANslation

---

## 🇬🇧 English

### What is FORTRAN?

FORTRAN is the oldest high-level programming language still in active use. Created in the late 1950s at IBM, it was revolutionary: for the first time, scientists and engineers could write programs in something resembling mathematical notation, instead of machine code or assembly. It was the proof that compilers could work — that a machine could translate human-readable code into efficient machine instructions.

FORTRAN did not just survive. It thrived. It is still used today in climate modeling, computational fluid dynamics, weather forecasting, nuclear physics, and high-performance scientific computing. Some FORTRAN codebases that run in production today were written before the programmers currently maintaining them were born.

This is not a museum exhibit. This is infrastructure.

---

### A brief history

| Year | Event |
|------|-------|
| 1954 | John Backus at IBM proposes the FORTRAN project |
| 1957 | FORTRAN I released — the first practical high-level language compiler |
| 1958 | FORTRAN II — subroutines, functions |
| 1962 | FORTRAN IV — broader standard |
| 1966 | FORTRAN 66 — first ANSI standard |
| 1978 | FORTRAN 77 — structured programming, `IF/THEN/ELSE`, `DO` loops with proper structure |
| 1991 | Fortran 90 — free-form source, modules, dynamic arrays, recursion. A major leap. |
| 1997 | Fortran 95 |
| 2004 | Fortran 2003 — object-oriented features, interoperability with C |
| 2010 | Fortran 2008 — coarrays for parallel computing |
| 2018 | Fortran 2018 — more parallelism, improved interoperability |

> The original IBM 704 had no high-level language. John Backus had to convince IBM management that a compiler was even worth building — most people believed a compiler could never produce code as efficient as hand-written assembly. FORTRAN proved them wrong. Every high-level language you use today owes something to that proof.

> **Note on capitalization**: The original language was written FORTRAN (all caps). Modern standards since Fortran 90 use "Fortran" (mixed case). You'll see both — they refer to the same language family.

---

### Best tools

| Tool | Purpose | Link |
|------|---------|-------|
| **GNU Fortran (gfortran)** | Free, excellent compiler — start here | https://gcc.gnu.org/fortran/ |
| **Intel oneAPI Fortran** | Industry-standard for HPC (free for community use) | https://www.intel.com/content/www/us/en/developer/tools/oneapi/fortran-compiler.html |
| **LFortran** | Modern, interactive Fortran compiler | https://lfortran.org |
| **VS Code** + Modern Fortran extension | Good editor support | https://marketplace.visualstudio.com/items?itemName=fortran-lang.linter-gfortran |
| **fpm** | Fortran Package Manager | https://fpm.fortran-lang.org |
| **Fortran-lang** | Community hub, resources, packages | https://fortran-lang.org |

---

### Basic syntax

> These examples use modern Fortran (Fortran 90 and later free-form style). Classic FORTRAN 77 required fixed column formatting — columns 1-5 for line numbers, column 6 for continuation, columns 7-72 for code. Modern Fortran abandons this.

#### Hello, World

```fortran
program hello
    implicit none
    print *, "Hello, World!"
end program hello
```

> `implicit none` is critical. Without it, FORTRAN has implicit typing rules (variables starting with i-n are integers, others are real). This rule has caused countless bugs. Always use `implicit none`.

#### Variables and types

```fortran
program variables
    implicit none

    ! Basic types
    integer            :: age = 30
    real               :: pi = 3.14159
    double precision   :: precise_pi = 3.14159265358979
    logical            :: is_active = .true.
    character(len=50)  :: name = "Alice"

    ! Modern Fortran — explicit kind
    integer, parameter :: dp = selected_real_kind(15, 307)  ! double precision kind
    real(kind=dp)      :: x = 1.0_dp

    ! Constants (PARAMETER)
    integer, parameter :: MAX_SIZE = 100
    real, parameter    :: GRAVITY = 9.81

    print *, "Name:   ", name
    print *, "Age:    ", age
    print *, "Pi:     ", pi
    print *, "Active: ", is_active

end program variables
```

#### Input / Output

```fortran
program io_example
    implicit none

    character(len=50) :: name
    integer           :: age

    write(*, *) "Enter your name:"
    read(*, *)  name

    write(*, *) "Enter your age:"
    read(*, *)  age

    write(*, '(A, A, A, I3, A)') "Hello, ", trim(name), "! You are ", age, " years old."

end program io_example
```

#### Conditionals

```fortran
program conditionals
    implicit none

    integer :: age

    age = 20

    if (age >= 18) then
        print *, "Adult"
    else if (age >= 13) then
        print *, "Teenager"
    else
        print *, "Child"
    end if

    ! SELECT CASE (like switch)
    select case (age)
        case (0:12)
            print *, "Child"
        case (13:17)
            print *, "Teenager"
        case (18:)
            print *, "Adult"
    end select

end program conditionals
```

#### Loops

```fortran
program loops
    implicit none

    integer :: i, sum

    ! DO loop (like for)
    do i = 1, 10
        print *, i
    end do

    ! DO with step
    do i = 0, 100, 10
        print *, i
    end do

    ! DO WHILE
    sum = 0
    i = 1
    do while (i <= 10)
        sum = sum + i
        i = i + 1
    end do
    print *, "Sum 1-10:", sum

    ! EXIT and CYCLE (break and continue)
    do i = 1, 20
        if (mod(i, 2) == 0) cycle    ! skip even numbers
        if (i > 15) exit              ! stop at 15
        print *, i
    end do

end program loops
```

#### Arrays — FORTRAN's strength

```fortran
program arrays
    implicit none

    integer, dimension(5) :: numbers = [1, 2, 3, 4, 5]
    real, dimension(3, 3) :: matrix
    integer :: i, j

    ! Array operations — whole array at once
    print *, "Sum:    ", sum(numbers)        ! 15
    print *, "Max:    ", maxval(numbers)     ! 5
    print *, "Min:    ", minval(numbers)     ! 1
    print *, "Product:", product(numbers)    ! 120

    ! Multiply all elements by 2 — no loop needed
    numbers = numbers * 2
    print *, numbers  ! 2 4 6 8 10

    ! 2D array (matrix)
    do i = 1, 3
        do j = 1, 3
            matrix(i, j) = real(i + j - 1)
        end do
    end do

    ! Dynamic arrays (allocatable) — modern Fortran
    block
        integer, allocatable :: dynamic(:)
        allocate(dynamic(10))
        dynamic = [(i * i, i = 1, 10)]  ! [1, 4, 9, 16, ..., 100]
        print *, dynamic
        deallocate(dynamic)
    end block

end program arrays
```

#### Subroutines and Functions

```fortran
program procedures
    implicit none

    real :: x, y, result_val
    x = 3.0
    y = 4.0

    result_val = hypotenuse(x, y)
    print *, "Hypotenuse:", result_val  ! 5.0

    call swap(x, y)
    print *, "After swap: x =", x, "y =", y

contains

    ! Function — returns a value
    real function hypotenuse(a, b)
        implicit none
        real, intent(in) :: a, b
        hypotenuse = sqrt(a**2 + b**2)
    end function hypotenuse

    ! Subroutine — modifies arguments via intent(inout)
    subroutine swap(a, b)
        implicit none
        real, intent(inout) :: a, b
        real :: temp
        temp = a
        a = b
        b = temp
    end subroutine swap

end program procedures
```

#### Modules — organizing code

```fortran
! math_utils.f90
module math_utils
    implicit none

    real, parameter :: PI = 3.14159265358979

contains

    real function circle_area(radius)
        implicit none
        real, intent(in) :: radius
        circle_area = PI * radius**2
    end function circle_area

    real function deg_to_rad(degrees)
        implicit none
        real, intent(in) :: degrees
        deg_to_rad = degrees * PI / 180.0
    end function deg_to_rad

end module math_utils


! main.f90
program main
    use math_utils
    implicit none

    real :: r = 5.0
    print *, "Area of circle with radius", r, ":", circle_area(r)
    print *, "90 degrees in radians:", deg_to_rad(90.0)

end program main
```

#### A real example — numerical integration

```fortran
! Numerical integration using the trapezoidal rule
! This is the kind of thing FORTRAN was BUILT for

program integration
    implicit none

    integer, parameter :: dp = selected_real_kind(15, 307)
    integer  :: n, i
    real(dp) :: a, b, h, x, result_val

    ! Integrate f(x) = x^2 from 0 to 1 (exact result = 1/3)
    a = 0.0_dp
    b = 1.0_dp
    n = 1000000   ! one million intervals

    h = (b - a) / real(n, dp)
    result_val = 0.5_dp * (f(a) + f(b))

    do i = 1, n - 1
        x = a + real(i, dp) * h
        result_val = result_val + f(x)
    end do

    result_val = result_val * h

    write(*, '(A, F20.15)') "Computed:  ", result_val
    write(*, '(A, F20.15)') "Exact 1/3: ", 1.0_dp / 3.0_dp

contains

    real(dp) function f(x)
        implicit none
        real(dp), intent(in) :: x
        f = x**2
    end function f

end program integration
```

---

## 🇫🇷 Français

### C'est quoi le FORTRAN ?

Le FORTRAN est le plus ancien langage de programmation de haut niveau encore en usage actif. Créé à la fin des années 1950 chez IBM, il était révolutionnaire : pour la première fois, des scientifiques et des ingénieurs pouvaient écrire des programmes ressemblant à de la notation mathématique, au lieu du code machine ou de l'assembleur. C'était la preuve que les compilateurs pouvaient fonctionner — qu'une machine pouvait traduire du code lisible par un humain en instructions machine efficaces.

Le FORTRAN n'a pas seulement survécu. Il a prospéré. Il est encore utilisé aujourd'hui dans la modélisation climatique, la mécanique des fluides numérique, les prévisions météorologiques, la physique nucléaire et le calcul scientifique haute performance. Certaines bases de code FORTRAN en production aujourd'hui ont été écrites avant la naissance des programmeurs qui les maintiennent.

Ce n'est pas une pièce de musée. C'est de l'infrastructure.

---

### Un peu d'histoire

| Année | Événement |
|-------|-----------|
| 1954 | John Backus chez IBM propose le projet FORTRAN |
| 1957 | FORTRAN I — le premier compilateur de langage de haut niveau pratique |
| 1966 | FORTRAN 66 — première norme ANSI |
| 1978 | FORTRAN 77 — programmation structurée |
| 1991 | Fortran 90 — forme libre, modules, tableaux dynamiques. Un bond en avant majeur. |
| 2004 | Fortran 2003 — fonctionnalités orientées objet |
| 2018 | Fortran 2018 — plus de parallélisme |

> John Backus a dû convaincre la direction d'IBM qu'un compilateur valait la peine d'être construit — la plupart des gens croyaient qu'un compilateur ne pourrait jamais produire du code aussi efficace que l'assembleur écrit à la main. FORTRAN leur a prouvé qu'ils avaient tort. Chaque langage de haut niveau que tu utilises aujourd'hui lui doit quelque chose.

---

### Les meilleurs outils

| Outil | Utilité | Lien |
|-------|---------|------|
| **GNU Fortran (gfortran)** | Compilateur gratuit et excellent — commence ici | https://gcc.gnu.org/fortran/ |
| **Intel oneAPI Fortran** | Standard industriel pour HPC | https://www.intel.com |
| **LFortran** | Compilateur Fortran moderne et interactif | https://lfortran.org |
| **Fortran-lang** | Hub communautaire | https://fortran-lang.org |

---

### Syntaxe de base

#### Hello, World

```fortran
program bonjour
    implicit none
    print *, "Bonjour, Monde !"
end program bonjour
```

> `implicit none` est critique. Sans lui, FORTRAN a des règles de typage implicites qui ont causé d'innombrables bugs. Utilise-le toujours.

#### Variables et types

```fortran
program variables
    implicit none

    integer            :: age = 30
    real               :: pi = 3.14159
    double precision   :: pi_precis = 3.14159265358979
    logical            :: est_actif = .true.
    character(len=50)  :: nom = "Alice"

    integer, parameter :: TAILLE_MAX = 100
    real, parameter    :: GRAVITE = 9.81

    print *, "Nom :", nom
    print *, "Âge :", age

end program variables
```

#### Boucles

```fortran
program boucles
    implicit none

    integer :: i, somme

    ! Boucle DO (comme for)
    do i = 1, 10
        print *, i
    end do

    ! DO avec pas
    do i = 0, 100, 10
        print *, i
    end do

    ! EXIT et CYCLE (break et continue)
    do i = 1, 20
        if (mod(i, 2) == 0) cycle   ! saute les pairs
        if (i > 15) exit            ! s'arrête à 15
        print *, i
    end do

end program boucles
```

#### Tableaux — la force du FORTRAN

```fortran
program tableaux
    implicit none

    integer, dimension(5) :: nombres = [1, 2, 3, 4, 5]

    ! Opérations sur tout le tableau à la fois — pas de boucle
    print *, "Somme    :", sum(nombres)       ! 15
    print *, "Max      :", maxval(nombres)    ! 5
    print *, "Produit  :", product(nombres)   ! 120

    nombres = nombres * 2   ! multiplie tout par 2
    print *, nombres         ! 2 4 6 8 10

end program tableaux
```

#### Un vrai exemple — intégration numérique

```fortran
! Intégration numérique par la règle des trapèzes
! Voilà pourquoi FORTRAN a été créé

program integration
    implicit none

    integer, parameter :: dp = selected_real_kind(15, 307)
    integer  :: n, i
    real(dp) :: a, b, h, x, resultat

    ! Intégrer f(x) = x^2 de 0 à 1 (résultat exact = 1/3)
    a = 0.0_dp
    b = 1.0_dp
    n = 1000000

    h = (b - a) / real(n, dp)
    resultat = 0.5_dp * (f(a) + f(b))

    do i = 1, n - 1
        x = a + real(i, dp) * h
        resultat = resultat + f(x)
    end do

    resultat = resultat * h

    write(*, '(A, F20.15)') "Calculé :  ", resultat
    write(*, '(A, F20.15)') "Exact 1/3: ", 1.0_dp / 3.0_dp

contains

    real(dp) function f(x)
        implicit none
        real(dp), intent(in) :: x
        f = x**2
    end function f

end program integration
```
