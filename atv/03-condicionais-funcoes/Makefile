all: ex1 ex2 ex3

%.o: %.c
	gcc -Og -fno-stack-protector -c $< -o $@

ex1: ex1.o solucao_ex1.o
	gcc solucao_ex1.o ex1.o -o ex1
	@./ex1 && echo "Todos os testes passaram!" || true
	@rm ex1

ex2: ex2.o solucao_ex2.o
	gcc solucao_ex2.o ex2.o -o ex2
	@./ex2 && echo "Todos os testes passaram!" || true
	@rm ex2

ex3: ex3.o solucao_ex3.o
	gcc solucao_ex3.o ex3.o -o ex3
	@./ex3 && echo "Todos os testes passaram!" || true
	@rm ex3

.PHONY = all
