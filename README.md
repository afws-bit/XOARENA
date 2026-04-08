# 🗺️ Caçador de Relíquias em C++

Este repositório contém o código-fonte de um jogo de aventura em terminal desenvolvido em C++, como parte das Entregas 01 e 02 da disciplina. O jogo consiste em navegar por um templo 5x5 usando um radar para encontrar um tesouro enquanto desvia de armadilhas mortais.

---

## 👥 Equipe e Papéis (Entrega 01)

| Nome do Integrante | Papel no Projeto | Responsabilidades Principais |
| :--- | :--- | :--- |
| **[Nome do Aluno 1]** | Product Owner (PO) | Definir as histórias de usuário, priorizar o backlog e garantir a entrega de valor. |
| **[Nome do Aluno 2]** | Scrum Master / Dev | Garantir a aplicação das práticas ágeis, remover impedimentos e auxiliar no desenvolvimento. |
| **[Nome do Aluno 3]** | Desenvolvedor | Codificar a lógica de geração de mapa e movimentação em C++. |
| **[Nome do Aluno 4]** | Desenvolvedor / QA | Desenvolver a interface no terminal, criar os diagramas e validar as regras de vitória/derrota. |

---

## 📋 Board e Backlog (Entrega 01)

O gerenciamento do projeto foi feito utilizando [Trello / Jira / GitHub Projects]. 
*As histórias abaixo estão listadas em ordem de prioridade (as mais críticas para o MVP do jogo estão no topo).*

> **Acesso ao Quadro:** [Insira o link para o seu quadro do Trello/Jira aqui]

### Prints Comprobatórios
*(Substitua os links abaixo pelas imagens reais dos seus prints do quadro)*
- ![Print do Quadro Kanban](link_para_imagem_do_quadro.png)
- ![Print do Backlog Priorizado](link_para_imagem_do_backlog.png)

---

## 📖 Histórias de Usuário (Padrão 3Cs)

Abaixo estão as 10 histórias de usuário definidas para o escopo do nosso jogo, formatadas com o padrão **Cartão (Card)**, **Conversação (Conversation)** e **Confirmação (Confirmation)**.

### US01: Exibição do Mapa Oculto (Prioridade Alta)
- **Cartão (Card):** Como jogador, quero visualizar o mapa na tela, mas com as áreas inexploradas cobertas por névoa, para que o jogo tenha mistério.
- **Conversação:** A matriz visual (`mapaTela`) será preenchida com caracteres `#`, exceto onde o jogador está (`P`). 
- **Confirmação (Critérios de Aceite):**
  1. O mapa 5x5 deve ser exibido a cada turno.
  2. Elementos ocultos (Relíquia e Armadilhas) não devem aparecer visualmente na impressão da tela.

### US02: Geração da Relíquia (Prioridade Alta)
- **Cartão (Card):** Como sistema, quero posicionar a relíquia em um local aleatório do mapa no início da partida para que o jogo seja diferente a cada jogatina.
- **Conversação:** Uso das funções `srand(time(0))` e `rand()` para definir as coordenadas X e Y da relíquia na matriz lógica, garantindo que não spawne na posição [0][0] onde o jogador nasce.
- **Confirmação:**
  1. A relíquia não pode iniciar nas coordenadas (0,0).
  2. A posição deve ser registrada na variável ou matriz de controle.

### US03: Movimentação (WASD) (Prioridade Alta)
- **Cartão (Card):** Como jogador, quero usar as teclas W, A, S, D para mover meu personagem ('P') pelas coordenadas da matriz.
- **Conversação:** Entrada do usuário será lida em um `char`. O `switch` ou `if` atualizará as coordenadas `jogadorX` e `jogadorY`.
- **Confirmação:**
  1. Tecla 'W' diminui o índice da linha. 'S' aumenta a linha.
  2. Tecla 'A' diminui o índice da coluna. 'D' aumenta a coluna.

### US04: Sistema de Radar (Prioridade Alta)
- **Cartão (Card):** Como jogador, quero receber uma dica de distância (radar) a cada turno para saber se estou indo na direção certa do tesouro.
- **Conversação:** O sistema calculará a distância de Manhattan: `abs(jogadorX - reliquiaX) + abs(jogadorY - reliquiaY)` e imprimirá a distância em número de passos.
- **Confirmação:**
  1. O texto do radar deve aparecer a cada movimentação.
  2. O número de passos deve diminuir se o jogador andar em direção à relíquia.

### US05: Validação de Bordas do Mapa (Prioridade Média)
- **Cartão (Card):** Como jogador, quero ser impedido de sair dos limites do mapa 5x5 para não quebrar o jogo.
- **Conversação:** Antes de atualizar a posição, verificar se a nova coordenada X ou Y será `< 0` ou `>= 5`. Se for, cancelar o movimento.
- **Confirmação:**
  1. Exibir a mensagem "Você bateu na parede do templo!" caso tente sair do mapa.
  2. A posição original do jogador não é alterada.

### US06: Condição de Vitória (Prioridade Alta)
- **Cartão (Card):** Como jogador, quero que o jogo anuncie minha vitória imediatamente ao pisar no bloco que contém a relíquia.
- **Conversação:** Após a movimentação, se a matriz lógica na posição `[jogadorX][jogadorY] == 'R'`, a variável booleana `venceu` fica verdadeira.
- **Confirmação:**
  1. O loop principal é quebrado.
  2. Mensagem de vitória é exibida com sucesso na tela.

### US07: Geração de Armadilhas (Prioridade Média)
- **Cartão (Card):** Como sistema, quero espalhar aleatoriamente 3 armadilhas pelo mapa para aumentar o desafio da exploração.
- **Conversação:** Loop que sorteia coordenadas e insere a letra 'T' na matriz lógica, validando para não sobrescrever a relíquia ou o início.
- **Confirmação:**
  1. A matriz de controle deve possuir exatamente três caracteres 'T'.
  2. As armadilhas não são reveladas na tela padrão.

### US08: Condição de Derrota (Prioridade Alta)
- **Cartão (Card):** Como jogador, quero que o jogo anuncie "Game Over" se eu pisar em uma armadilha oculta.
- **Conversação:** Se a matriz lógica na posição `[jogadorX][jogadorY] == 'T'`, a variável booleana `vivo` fica falsa.
- **Confirmação:**
  1. O loop principal é quebrado instantaneamente.
  2. Mensagem de derrota (KABUM!) é exibida.

### US09: Rastro de Exploração (Prioridade Baixa)
- **Cartão (Card):** Como jogador, quero ver os lugares por onde já passei marcados com um '.' (ponto) para não me perder.
- **Conversação:** Ao sair de uma coordenada atualizada com sucesso, o sistema substitui o antigo 'P' e a antiga '#' por um '.' na tela.
- **Confirmação:**
  1. Células já visitadas não mostram mais o caractere '#'.
  2. Células visitadas exibem '.' mostrando que estão seguras.

### US10: Interface e Comandos Inválidos (Prioridade Baixa)
- **Cartão (Card):** Como jogador, quero ser notificado e manter meu turno caso eu digite uma tecla diferente de W, A, S ou D.
- **Conversação:** Um `else` final na checagem de movimento vai capturar entradas estranhas (como X, K, 5).
- **Confirmação:**
  1. Exibir "Comando inválido!".
  2. O jogador não se move e não ativa armadilhas nessa rodada.

---

## 📊 Diagramas (Entrega 02)

Para a Entrega 02, cada História de Usuário (US) possui um diagrama simplificado (UML/Fluxograma) ilustrando sua execução no código.

*Nota: Você deve criar os diagramas (ex: Draw.io) e salvar na pasta `/diagramas` do repositório.*

- [Diagrama US01 - Exibir Mapa com Névoa](./diagramas/US01.png)
- [Diagrama US02 - Spawn da Relíquia](./diagramas/US02.png)
- [Diagrama US03 - Inputs e Movimento WASD](./diagramas/US03.png)
- [Diagrama US04 - Cálculo do Radar](./diagramas/US04.png)
- [Diagrama US05 - Colisão com Parede](./diagramas/US05.png)
- [Diagrama US06 - Verificação de Vitória](./diagramas/US06.png)
- [Diagrama US07 - Spawn das Armadilhas](./diagramas/US07.png)
- [Diagrama US08 - Verificação de Derrota](./diagramas/US08.png)
- [Diagrama US09 - Atualização de Rastro Visual](./diagramas/US09.png)
- [Diagrama US10 - Tratamento de Tecla Inválida](./diagramas/US10.png)
