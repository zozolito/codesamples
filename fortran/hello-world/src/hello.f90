! ============================================================
! HELLO WORLD — GNU Fortran sample project
! Build : Ctrl+Shift+B
! Run   : Ctrl+F5  (or open a terminal: ./bin/hello)
! Debug : F5
! ============================================================
program hello_world

    implicit none  ! always — no implicit typing

    ! ── Variables ──────────────────────────────────────────
    character(len=50) :: user_name
    integer           :: counter
    real              :: pi = 3.14159265
    real              :: radius, area

    ! ── Banner ─────────────────────────────────────────────
    call print_banner()

    ! ── Interactive greeting ────────────────────────────────
    write(*, '(A)', advance='no') 'Enter your name: '
    read(*, '(A)') user_name

    do counter = 1, 3
        write(*, '(I1, A, A, A)') counter, '. Hello, ', trim(user_name), '!'
    end do

    write(*, *)  ! blank line

    ! ── A taste of what Fortran does best: maths ───────────
    write(*, '(A)', advance='no') 'Enter a circle radius: '
    read(*, *) radius

    area = pi * radius**2

    write(*, '(A, F6.2, A, F10.4)') &
        'Area of circle with radius ', radius, ' = ', area

    write(*, *)
    write(*, '(A)') 'Welcome to Fortran. The language that sent humans to the Moon.'

contains

    ! ── Internal subroutine ─────────────────────────────────
    subroutine print_banner()
        write(*, '(A)') '============================================'
        write(*, '(A)') '  Fortran Hello World — CodeSamples Project '
        write(*, '(A)') '============================================'
        write(*, *)
    end subroutine print_banner

end program hello_world
