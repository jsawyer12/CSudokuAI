CFLAGS = -Wall -Wextra

clean: 
	rm -f sudoku_solver sudoku_check sudoku_advanced

sudoku_check: sudoku_check.c sudoku_io.c
	gcc ${CFLAGS} -o sudoku_check sudoku_check.c

sudoku_solver: sudoku_solver.c sudoku_io.c
	gcc ${CFLAGS} -o sudoku_solver sudoku_solver.c

sudoku_advanced: sudoku_advanced.c sudoku_io_ext.c
	gcc ${CFLAGS} -o sudoku_advanced sudoku_advanced.c