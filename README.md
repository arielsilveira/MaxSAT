# MaxSAT

Trabalho final de Metaheurística onde implementa os algorítmos Simulated Annealing, ILS e Multistart para resolver o problema MaxSat-3.

## Execução
Para executar o programa, pode utilizar os comandos do Makefile.

É possível executar o comando com 20 átomos:

`
$ make run20
` 

O seguinte comando executa com 250 átomos:

`
$ make run250
` 

Caso deseja aplicar para outros arquivos, a pasta `input` contém outros diferentes tipos de entrada que podem ser aplicados, deve ser passado como argumento de execução o seguinte formato.

`<pasta_do_arquivo>/<nome_do_arquivo>`

Também pode ser visto como exemplo o arquivo `Makefile`.