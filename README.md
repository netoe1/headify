# Headify

Headify é uma ferramenta de linha de comando em C que extrai declarações de arquivos-fonte anotados e gera um arquivo de cabeçalho (`.h`) e uma cópia processada do fonte (`.c`). As anotações são palavras-chave colocadas no próprio código.

## Requisitos

- Um compilador C com suporte a C11 e à função POSIX `getline`.
- `make` para compilar usando o arquivo `makefile` incluído.

## Compilar

Na raiz do repositório, execute:

```sh
make
```

Isso cria o executável `headify`. Para removê-lo:

```sh
make clean
```

## Uso

```sh
./headify generate <entrada.headify> <saida.h>
```

Exemplo:

```sh
./headify generate src/math.headify include/math.h
```

O comando cria:

- `include/math.h`, no caminho informado;
- `src/math.headify-generated.c`, ao lado do arquivo de entrada. Esse fonte inclui `math.h` pelo nome.

Os diretórios de saída precisam existir antes da execução. O arquivo de entrada deve terminar em `.headify`, e o caminho do cabeçalho em `.h`.

Ajuda e licença:

```sh
./headify --help
./headify -h
./headify --LICENSE
```

## Anotações disponíveis

Uma anotação pode ter espaços ou tabulações antes dela, mas deve ser o primeiro conteúdo da linha.

| Anotação | Resultado no cabeçalho | Resultado no fonte gerado |
| --- | --- | --- |
| `@var` | Copia o restante da linha como declaração | Remove a linha |
| `@function` | Gera um protótipo a partir da assinatura | Remove `@function` e mantém a função |
| `@comment` | Copia o restante da linha como comentário | Remove a linha |
| `@struct` | Copia a definição da struct até o fechamento | Remove a definição |
| `@define` | Converte o restante da linha em `#define` | Remove a linha |

### Variável global

```c
@var extern int total;
```

O cabeçalho recebe `extern int total;`. A linha anotada não aparece no fonte gerado; coloque a definição da variável em outro ponto do projeto quando necessário.

### Função

```c
@function int soma(int a, int b) {
    return a + b;
}
```

O cabeçalho recebe:

```c
int soma(int a, int b);
```

O fonte gerado mantém a implementação, removendo o marcador.

### Comentário para o cabeçalho

```c
@comment // Soma dois valores inteiros.
@function int soma(int a, int b) {
    return a + b;
}
```

O comentário é colocado no cabeçalho; a linha `@comment` é omitida do fonte gerado.

### Struct

```c
@struct struct Pessoa {
    char nome[64];
    int idade;
};
```

A definição completa é copiada para o cabeçalho e removida do fonte gerado.

### Macro

```c
@define MAX_ITENS 128
```

O cabeçalho recebe `#define MAX_ITENS 128`.

## Limitações

- O Headify processa o arquivo linha a linha; não é um parser completo da linguagem C.
- A assinatura marcada com `@function` deve estar na mesma linha que o marcador. A implementação pode continuar nas linhas seguintes.
- O reconhecimento do fim de funções e structs usa chaves (`{` e `}`) e não interpreta comentários ou literais que contenham esses caracteres.
- `@var` copia a linha para o cabeçalho e a remove do fonte gerado. Inicializadores nessa linha também vão para o cabeçalho; em projetos com várias unidades de compilação, prefira uma declaração `extern` e mantenha a definição em um arquivo-fonte.
- O fonte gerado inclui o cabeçalho apenas pelo nome do arquivo. Organize os diretórios ou inclua caminhos de busca adequados ao compilar o projeto.

## Licença

Distribuído sob a licença MIT. Consulte [LICENSE](LICENSE) para o texto completo e as informações de autoria.
