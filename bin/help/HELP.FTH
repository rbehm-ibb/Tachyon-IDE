TACHYON V4 ( V4.5 )

pub HELP.FTH		PRINT" Help and hint extensions for Tachyon SD card systems 170713-1030 " ;

{
HELP uses a collection of help files located in the HELP directory.
A HELP.TXT is opened and scanned for the word according to a simple procedure which
then indicates which help file to open.
This help file is then opened and then the file is once again scanned for this word so that
the line may be extracted by HINT or else highlighted as the whole file is displayed.
Page control is optional for smaller displays or larger files.

HELP.TXT example
LOOPS.TXT	FOR NEXT FROM BY FOR! FOR@ BY! LP! I J ADO LOOP+ LEAVE
PINIO.TXT	LOW HIGH FLOAT OUTCLR OUTSET OUTPUTS H L N P F PIN@ IN

Each HELP file has the keyword indented by one space at the start of a line to aid scanning.
Lines that do not have a single space can be skipped.

Structure of a help file:
LOOPS.TXT
 FOR ( cnt -- )
Enter a FOR NEXT loop which will loop for cnt times.
A default index FROM 0 and a index step BY 1 is used unless these are overridden before FOR using FROM and/or BY.

 NEXT
:
A blank line indicates end of paragraph so a character such as + must be used to indicagte continuation.






Notes: GETWORD is being deprecated in V4.5 in favor of deferred words causing unknown words to be converted to strings.
This preserves the flow of the input stream rather than diverting to GETWORD.
Example: FOPEN myfile  --- FOPEN is deferred to execute at the end of the line so myfile is converted to a string which
the deferred FOPEN can then use without interrupt the input stream.
}



pre HELP ( <name> -- )
	DEFER
	1! 0 2! ( save address of string and index)
	" HELP.TXT" FOPEN$
	IF
	  BEGIN 2@ WORD@ DUP 1@ <> WHILE LEN$ 2@ + 2! REPEAT
	  --- backtrack to start of line to find help file to use
	  2@ BEGIN DUP C@ $0D <> WHILE 1- REPEAT
	  --- and list the whole help file (highlighting to be added etc)
	  1+ WORD@ FPRINT$
	ELSE
	  PRINT"  - Sorry, that word was not found in the help file "
	THEN
	;

--- Display one line stack and description hint
pre HINT ( <name> -- )
	;

--- Display the source code for this word from file
pre SOURCE ( <name> -- )

	;

--- Find and display all references to word
pre WHEREIS ( <name> -- )

	;


END
