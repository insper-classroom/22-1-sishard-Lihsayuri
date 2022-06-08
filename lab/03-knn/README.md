# K-Nearest Neighbors Classifier

O KNN (k-vizinhos mais próximos) é um método que foi desenvolvido para classificação. A ideia é que classificar um novo registro, com base no suposto de que registros com resposta parecida possuem características igualmente parecidas,  Dessa forma, o KNN busca encontrar k vizinhos que sejam mais semelhantes ao registro que se deseja classificar.


E para fazer isso, uma ideia é separar o processo em threads diferentes. Isso à primeira vista pode não parecer tão vantajoso, mas quando comparamos a execução para um registro grande feito com apenas 1 thread com a execução de várias threads simultâneas é que a diferença torna-se visível. Caso você não esteja acreditando, basta testar. Neste repositório você encontrará vários exemplos para rodar o código. Para isso basta executar em seu terminal:

gcc -Wall -Wno-unused-result -g classifier.c knn.o -o classifier -lm -pthread -lraylib

./classifier -k <número de vizinhos> -d <path do csv de treino> -t <path do csv de teste> -n <número de threads desejadas>

Um exemplo seria:

./classifier -k 3 -d ex_10000-size_7-classes_15-features_train.csv  -t ex_10000-size_7-classes_15-features_test.csv -n 10

### Em relação as rubricas

- C

Para fazer a rubrica C, eu implementei a separação dos argumentos no terminal para que, independente da ordem escrita, o usuário pudesse informar o número de vizinhos a serem considerados (-k <número de vizinhos>), informar qual arquivo utilizar para o treinamento (-d <path do csv de treino>) e informar qual arquivo utilizar para teste (-t <path do csv de teste>). Para este conceito, como o usuário não escolhe o número de threads que podem ser executadas simultâneamente, foi definido um padrão de 100.  

Nesse caso, o programa treina com a função fit o KNN na main e faz o predict do KNN tanto na base de treino, quanto na de teste, e no final mostra a acurácia de cada uma delas.

Para fazer o predict, foram utilizadas uma thread para cada linha

Além disso, o programa compila sem warnings. 

- B 

Para fazer a rubrica B, eu implementei a função do usuário escolher através do terminal o número de threads que gostaria de rodar simultaneamente. Assim, a partir disso, toda vez que uma thread for executada, ela irá parar na *thread_barreira, que irá fazer a checagem. A ideia foi fazer isso a partir de um semáforo, que enquanto não forem 0, irão ficar rodando quantas threads puderem. Mas, quando os semáforos forem para 0, as threads param de ser executadas até que uma delas libere o semáforo para outra ser executada, e assim por diante. 

Além disso, o programa compila sem warnings. 


- A

Para o conceito A, uma janela gráfica foi criada para mostra uma barra de progresso da etapa do predict, tanto para a base de treino, quando para a base de testes. Além disso, fiz como extra a barra de progresso do processo de criação das threads. Por fim, no final os resultados de acurácia do teste e do treino também são mostrados para o usuário.

Para entender um pouco mais sobre o funcionamento e ver o código rodando, abaixo o link do vídeo gravado que mostra como tudo funciona:

https://drive.google.com/file/d/1NyHlGNSq-mHJS2Yx2lH8LyeuhRk_mTBQ/view?usp=sharing