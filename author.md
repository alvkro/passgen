# Introduction

O Passgen é um projeto que consiste em um gerador de senhas utilizando argumentos CLI (Command Line Interface), no qual o usuário pode customizar a senha no momento que executa o programa, inserindo parâmetros, tais como letra maiúscula, minúscula, logogramas, tamanho da senha desejada e até mesmo a entropia (força) da senha.

Breve descrição do projeto.

# Author(s)

Alvaro Barbosa Filho
Gabriel Garcia Lima

alvaro.filho.111@ufrn.edu.br
ggdelima22@gmail.com 

# Problems found or limitations

(Um) dos maiores problemas encontrados foi a questão de qual estrutura utilizar para armazenar os grupos de caracteres, inicialmente fizemos um ´map <char, bool>´ para cada um dos grupos que seriam utilizados, só que seria inviável pois seria necessário acessar a chave referente à aquele grupo, então descartamos e fizemos uma estrutura "brute-force", com cada uma deles sendo strings contendo os grupos respectivos, junto de um unordered set para evitar repetições caso o usuário adicionasse múltiplas vezes a mesma flag. Única limitação do projeto é o requisito de usar as iniciais como parâmetro tal qual `-ludopq`, que não conseguimos achar uma forma de implementar isso.

O gerador também foi desafiador de se implementar seguindo as especificações pedidas no arquivo, no qual seria necessário conferir se todos os grupos foram utilizados.

Além disso, a correção de bugs que eram quase silenciosos, que só foram descobertos durante a fase de diversos testes, que ao comparar com os cases do `README.md` foi notado uma discrepância, ao investigar, foi percebido que havia-se errado no cálculo da entropia da senha, sendo necessário refatorar uma boa parte do código para se adequar aos parâmetros estabelecidos. Outro bug que vale a pena se comentar era caso o usuário digitasse a mesma flag múltiplas vezes, o que ocasionaria em vários bugs, algo que foi corrigido!

Por último, a estrutura do código tem o aspecto de ser "gambiarrento". Na função validate_arguments, tem um if-else encadeado até o final da função praticamente, até o momento não se sabe quão problemático isso pode ser, mas o importante é que está funcionando.

*IMPORTANTE:* A documentação Doxygen foi gerada utilizando IA.

# Grading

Preencha os itens que você implementou, indicando qual a nota esperada para cada item.

| Item                                             | Valor máximo | Valor esperado |
| ------------------------------------------------ | :----------: | :------------: |
| Interface de acordo com o especificado           |      20      |        20        |
| Seleciona caracteres dos grupos adequadamente    |      15      |         10       |
| Apresenta a tela de ajuda quando solicitado      |      5       |         5       |
| Trata os erros de entrada corretamente           |      12      |         12       |
| Avalia a qualidade da senha com 5 categorias     |      10      |          10      |
| Implementa os 9 grupos de caracteres solicitados |      18      |        16        |
| Utiliza corretamente a aleatoriedade             |      10      |         9       |

# Compiling and Running

Para compilar o programa, basta usar o GCC dentro da pasta src:

`g++ main.cpp -o passgen`

Para utilizar:
`./passgen <cli-arguments>`

Dica: Para saber quais argumentos válidos, usar `./passgen -h (ou --help)`
