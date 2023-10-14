# Identificação Pessoal

- Nome: _José Davi Viana Francelino_
- Email: *jose.davi@imd.ufrn.br*
- Turma: _DIM0605 - T01_

- Nome: _Thiago de Oliveira Cordeiro_
- Email: *thiagoty52@gmail.com*
- Turma: _DIM0605 - T01_

# Compilando e executando

## Manualmente com g++

Para compilar e executar o programa manualmente basta digitar os seguintes comandos em seu terminal:

```
$ git clone https://github.com/ThiagoCordeiro52/sudoku-solver (faz o clone do repositório em algum diretório do seu pc)
$ cd sudoku-solver/ (vai até a pasta do repositório clonado)
$ mkdir build (caso não tenha uma pasta para os executáveis, você deve criá-la com esse comando)
$ g++ -Wall -std=c++20 -g src/main.cpp src/graph.cpp src/fileio.cpp -I src/include -o build/sudoku (compilar)
$ ./build/sudoku <input_content> (executar)
```

## Cmake

Para compilar e executar o programa com o Cmake você precisa ter instalado pelo menos a versão 3.26. Em seguida, abra seu terminal e execute os seguintes comandos:

```
$ git clone https://github.com/ThiagoCordeiro52/sudoku-solver (faz o clone do repositório em algum diretório do seu pc)
$ cd sudoku-solver/ (vai até a pasta do repositório clonado)
$ mkdir build (caso não tenha uma pasta para os executáveis, você deve criá-la com esse comando)
$ cmake -S src -B build (criar o cmake do projeto)
$ cmake --build build (compilar)
$ ./build/sudoku <input_content> (executar)
```

---

&copy; IMD/UFRN 2023.
