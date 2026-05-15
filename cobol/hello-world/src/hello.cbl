      *> ============================================================
      *> HELLO WORLD — GnuCOBOL sample project
      *> Run  : Ctrl+Shift+B  (Build Task)  then  Ctrl+F5
      *> Debug: F5
      *> ============================================================
       IDENTIFICATION DIVISION.
       PROGRAM-ID. HELLO-WORLD.
       AUTHOR. CodeSamples Community.

       DATA DIVISION.
       WORKING-STORAGE SECTION.
           01 WS-NAME          PIC X(30) VALUE SPACES.
           01 WS-COUNTER       PIC 9(2)  VALUE 1.
           01 WS-GREETING      PIC X(50) VALUE SPACES.

       PROCEDURE DIVISION.

           PERFORM DISPLAY-BANNER

      *>   Ask for the user's name
           DISPLAY "Enter your name: " WITH NO ADVANCING
           ACCEPT WS-NAME

      *>   Build a greeting and display it 3 times
           STRING "Hello, " DELIMITED SIZE
                  FUNCTION TRIM(WS-NAME) DELIMITED SIZE
                  "!" DELIMITED SIZE
               INTO WS-GREETING

           PERFORM VARYING WS-COUNTER FROM 1 BY 1
               UNTIL WS-COUNTER > 3
               DISPLAY WS-COUNTER ". " FUNCTION TRIM(WS-GREETING)
           END-PERFORM

           DISPLAY " "
           DISPLAY "Welcome to COBOL. The language that runs the world."

           STOP RUN.

      *> ── Paragraphs ───────────────────────────────────────────────

       DISPLAY-BANNER.
           DISPLAY "============================================"
           DISPLAY "  COBOL Hello World — CodeSamples Project  "
           DISPLAY "============================================"
           DISPLAY " ".
