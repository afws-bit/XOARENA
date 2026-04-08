# Projeto Caçador de Relíquias em C++

Este repositório contém o código-fonte de um jogo de aventura em terminal desenvolvido em C++, como parte das Entregas 01 e 02 da disciplina. O sistema consiste em um motor de exploração onde o usuário navega por um ambiente 5x5 em busca de um objetivo oculto, utilizando lógica de detecção de proximidade.

---

## Equipe 

|Nome dos Integrantes| 
 • Augusto Freitas  
 • João Lucas mendes 
 • Caio Catão  
 • Fernando Araújo 
 • Glauberson  
 • Gustavo Cassemiro  
 • Daniel Massud  

---

## Gerenciamento do Projeto: Board e Backlog 

O gerenciamento do ciclo de vida do projeto foi realizado via [Trello / Jira / GitHub Projects]. As histórias de usuário estão listadas abaixo seguindo a prioridade técnica para o funcionamento do sistema (MVP).

> Acesso ao Quadro: https://linear.app/xoarena/team/JOG/backlog

### Evidências do Processo Ágil
- Imagem do Quadro Kanban: <img width="1043" height="561" alt="Captura de tela 2026-04-08 171705" src="https://github.com/user-attachments/assets/0ece994c-a48b-4349-84e1-91987cbb3a6f" />


---

## Histórias de Usuário 

Abaixo constam as 10 histórias de usuário detalhando as funcionalidades do sistema.

### US01: Visualização do Mapa com Névoa
- Card: Como jogador, desejo visualizar o mapa com áreas inexploradas ocultas para que o jogo apresente desafio de exploração.
- Conversation: O mapa em tela utilizará o caractere '#' para representar áreas não visitadas e 'P' para a posição atual.
- Confirmation: O grid 5x5 deve ser impresso a cada turno ocultando a relíquia e as armadilhas.

### US02: Posicionamento Aleatório da Relíquia
- Card: Como sistema, devo posicionar o objetivo em coordenadas aleatórias para garantir a rejogabilidade.
- Conversation: Utilização de sementes de tempo (srand) para definir X e Y, evitando a posição inicial (0,0).
- Confirmation: A relíquia não deve ocupar a posição de spawn do jogador.

### US03: Interface de Movimentação (WASD)
- Card: Como jogador, desejo utilizar as teclas W, A, S e D para controlar o personagem.
- Conversation: O sistema interpretará caracteres de entrada para incrementar ou decrementar os índices da matriz.
- Confirmation: O personagem deve atualizar sua posição conforme a tecla pressionada.

### US04: Sistema de Detecção (Radar)
- Card: Como jogador, desejo receber informações de distância do objetivo para orientar minha navegação.
- Conversation: Cálculo da distância de Manhattan entre as coordenadas do jogador e da relíquia.
- Confirmation: O valor da distância deve ser exibido de forma legível após cada movimento.

### US05: Restrição de Limites (Colisões)
- Card: Como sistema, devo impedir que o jogador ultrapasse as bordas do mapa.
- Conversation: Implementação de condicionais que validam se a nova coordenada está entre 0 e 4.
- Confirmation: O sistema deve exibir mensagem de erro e manter o jogador na posição atual caso tente sair do grid.

### US06: Condição de Vitória
- Card: Como jogador, desejo ser notificado da vitória ao alcançar a coordenada da relíquia.
- Conversation: Verificação de igualdade entre as coordenadas do jogador e do objetivo após o movimento.
- Confirmation: O loop do jogo deve ser encerrado e uma mensagem de sucesso exibida.

### US07: Implementação de Armadilhas
- Card: Como sistema, devo inserir obstáculos ocultos no mapa para aumentar o risco da exploração.
- Conversation: Alocação de três pontos de falha (T) na matriz lógica de forma aleatória.
- Confirmation: As armadilhas devem permanecer ocultas ao jogador até que ocorra a colisão.

### US08: Condição de Derrota
- Card: Como jogador, desejo que o jogo encerre caso eu encontre uma armadilha.
- Conversation: Verificação de colisão com caracteres de dano na matriz lógica.
- Confirmation: Exibição de mensagem de Game Over e encerramento do processo.

### US09: Rastro de Navegação
- Card: Como jogador, desejo que o mapa registre as áreas já exploradas.
- Conversation: Substituição do caractere '#' por '.' em todas as coordenadas já visitadas.
- Confirmation: O rastro visual deve ser persistente durante a sessão de jogo.

### US10: Tratamento de Entradas Inválidas
- Card: Como sistema, devo ignorar comandos que não pertençam ao conjunto de movimentação.
- Conversation: Implementação de cláusula default no tratamento de inputs para capturar teclas incorretas.
- Confirmation: O jogo não deve processar o turno ou mover o jogador caso a tecla seja inválida.

---

## Design e Prototipagem 

### Demonstração do Protótipo (Screencast)
- Link para o vídeo: 

### Protótipo de Baixa Fidelidade (Figma)
- Link para o projeto: 


Tela do game:

<img width="1024" height="559" alt="image" src="https://github.com/user-attachments/assets/bd2f4d7c-1621-4551-ba2c-d037d35e96d9" />


