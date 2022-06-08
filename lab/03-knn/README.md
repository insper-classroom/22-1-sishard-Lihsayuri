## K-Nearest Neighbors Classifier :pushpin: :bar_chart: <img src="https://img.shields.io/static/v1?label=EntregaLab3&message=Finalizado&color=success&style=flat-square&logo=ghost"/>

O KNN (k-vizinhos mais próximos) é um método que foi desenvolvido para classificação. A ideia é que classificar um novo registro, com base no suposto de que registros com resposta parecida possuem características igualmente parecidas,  Dessa forma, o KNN busca encontrar k vizinhos que sejam mais semelhantes ao registro que se deseja classificar.


E para fazer isso, uma ideia é separar o processo em threads diferentes. Isso à primeira vista pode não parecer tão vantajoso, mas quando comparamos a execução para um registro grande feito com apenas 1 thread com a execução de várias threads simultâneas é que a diferença torna-se visível. Caso você não esteja acreditando, basta testar. Neste repositório você encontrará vários exemplos para rodar o código. Para isso basta executar em seu terminal:

```C
gcc -Wall -Wno-unused-result -g classifier.c knn.o -o classifier -lm -pthread -lraylib
```
Ess linha irá compilar o seu código.

```C
./classifier -k <número_de_vizinhos> -d <path_do_csv_de_treino> -t <path_do_csv_de_teste> -n <número_de_threads_desejadas>
```
E essa linha irá executar algum dos exemplos disponíveis que você irá encontrar na pasta. Para facilitar a sua vida, um exemplo seria:

```C
./classifier -k 3 -d ex_10000-size_7-classes_15-features_train.csv  -t ex_10000-size_7-classes_15-features_test.csv -n 10
```
Aí basta trocar apenas algumas variáveis para ir testando com os vários exemplos :wink: .


## Ideia geral :clipboard:

Antes de falar do que foi feito em cada conceito do LAB, vamos entender um pouco da ideia geral. 

A ideia geral é: realizar as predições de cada linha do documento em diferentes threads, utilizar semáforo para controlar quantas threads podem ser executadas simultaneamente e ir iterando o processo para o usuário através de barras de progresso dos processos de criação e execução das threads, tanto para as threads de treino, quanto para as threads de teste. Por fim, uma tela final mostra os resultados obtidos para a acurácia do treino e do teste.  

Para o primeiro passo que é utilizar semáforo para controlar o número de threads, basta pensar que ao inicializá-lo com algum valor, isso significa - pensando em uma analogia - que ele possui um saldo positivo de tickets, como foi feito na parte do código que pode ser vista abaixo:

```C
sem_init(lista_test->sem_barrier, 0, n_threads); //valor maximo de semaforos rodando 
```

No caso, esse valor de tickets é definido pelo usuário. E toda vez que a thread passar pela função  ```sem_wait(sem_t *semaforo)```, um desses tickets é tirado. Ou seja, enquanto houverem tickets disponíveis, as threads irão passar pelo semáforo e executar normalmente, de maneira simultânea. Entretanto, se o valor desse saldo foi 0, isso significa que mais nenhuma thread poderá rodar enquanto o saldo não for positivo. E como o salfo voltará a ser positivo? Através da função ```sem_post(sem_t *semaforo)```, que faz exatamente o contrário da função ```sem_wait(sem_t *semaforo)```, ou seja, adiciona um ticket ao semáforo. Assim, conforme as threads vão passando pela  ```*thread_barreira(void *args)``` - que é quem faz o controle do semáforo - elas vão tirando tickets do semáforo, e assim que executam, também vão adicionando tickets. 

E todas essas informações do semáforo são passadas para a ```*thread_barreira(void *args)``` através de uma struct chamada ```barrier```, que também possui informações relacionadas ao número da thread que está rodando e os dados que são recebidos na função ```knn_predict(arg->knn_model, arg->data)```. 

Agora, em relação ao próximo passo que envolve a janela gráfica, uma outra struct foi criada - a ```loading```. Essa struct um contador que indica qual thread está rodando, o total de threads que será rodada e um indicar para saber se estamos no processo de treinamento ou teste. Assim, com todas essas informações que serão passadas nas threads:  ```*thread_desenha_run(void *args)``` e ```*thread_desenha_create(void *args)```, podemos saber qual thread está rodando em relação ao total. 
Logo, graficamente foi feito apenas um retângulo, que vai sendo preenchido conforme essa relação vai se aproximando até ser igual.


## Em relação as rubricas :chart_with_upwards_trend: :school:

### -C 

&nbsp; &nbsp; &nbsp; :heavy_check_mark: Para fazer a rubrica C, eu implementei a separação dos argumentos no terminal para que, independente da ordem escrita, o usuário pudesse informar o número de vizinhos a serem considerados (-k <número_de_vizinhos>), informar qual arquivo utilizar para o treinamento (-d <path_do_csv_de_ treino>) e informar qual arquivo utilizar para teste (-t <path_do_csv_de_teste>). Para este conceito, como o usuário não escolhe o número de threads que podem ser executadas simultâneamente, foi definido um padrão de 100 - definido no início da função main():
  
```C
 int n_threads = 100; // padrão caso não seja passado nada
```
 
&nbsp; &nbsp; &nbsp; :heavy_check_mark: Nesse caso, o programa treina com a função fit o KNN na main, com o número de vizinhos definido pelo usuário, e faz o predict do KNN tanto na base de treino, quanto na de teste, e no final mostra a acurácia de cada uma delas.

&nbsp; &nbsp; &nbsp; :heavy_check_mark: Para fazer o predict, foram utilizadas uma thread para cada linha, tanto para o treino quanto para os testes.

&nbsp; &nbsp; &nbsp; :heavy_check_mark: Além disso, o programa compila sem warnings. 

### -B 

&nbsp; &nbsp; &nbsp; :heavy_check_mark: Para fazer a rubrica B, eu implementei a função do usuário escolher através do terminal o número de threads que gostaria de rodar simultaneamente. Assim, a partir disso, toda vez que uma thread for executada, ela irá parar na ```*thread_barreira```, que irá fazer a checagem. A ideia foi fazer isso a partir de um semáforo, que enquanto não for 0, irá ficar rodando quantas threads puder. Mas, quando os semáforos forem para 0, as threads param de ser executadas até que uma delas libere o semáforo para outra ser executada, e assim por diante. 

&nbsp; &nbsp; &nbsp; :heavy_check_mark: Além disso, o programa compila sem warnings. 

### -A 

&nbsp; &nbsp; &nbsp; :heavy_check_mark: Para o conceito A, uma janela gráfica foi criada para mostra uma barra de progresso da etapa do predict, tanto para a base de treino, quando para a base de testes. Além disso, fiz como extra a barra de progresso do processo de criação das threads. Por fim, no final os resultados de acurácia do teste e do treino também são mostrados para o usuário.

&nbsp; &nbsp; &nbsp; :heavy_check_mark: Para entender um pouco mais sobre o funcionamento e ver o código rodando, abaixo o link do vídeo gravado que mostra como tudo funciona: https://drive.google.com/file/d/1NyHlGNSq-mHJS2Yx2lH8LyeuhRk_mTBQ/view?usp=sharing

## Referências

:copyright: Feito por: Lívia Sayuri Makuta. Insper. Engenharia de Computação - 5º Semestre. 

:round_pushpin: Matéria: Sistemas Hardware- Software.  :raising_hand_man: Professor: Maciel Calebe. 
