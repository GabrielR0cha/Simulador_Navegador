# Simulador_Navegador
# 🌐 Navegador com Utilização de Listas Encadeadas, Pilhas e Filas

Este projeto simula o funcionamento de um navegador utilizando **listas encadeadas**, **pilhas** e **filas**. O sistema oferece funcionalidades avançadas como manipulação de páginas, histórico, favoritos e controle de desempenho em termos de complexidade computacional.

---

## 📋 Funcionalidades

### 📑 Manipulação de Páginas (Lista Duplamente Encadeada)
- **Adicionar**: Insere uma nova página ao navegador.
- **Remover**: Exclui uma página atual, armazenando-a no histórico.
- **Buscar**: Localiza páginas por título ou URL.
- **Listar**: Exibe todas as páginas abertas em ordem.

### 🕑 Histórico de Páginas (Pilha)
- **Armazenar**: Salva as páginas removidas.
- **Visualizar**: Permite acessar as páginas excluídas.
- **Recuperar**: Reinsere páginas do histórico na lista de navegação.

### ⭐ Favoritos (Fila)
- **Adicionar**: Marca páginas como favoritas.
- **Listar**: Exibe os favoritos em ordem de adição.
- **Recuperar**: Move favoritos de volta para as páginas principais.

### 🕒 Tempo e Complexidade (Big-O)
- Cada funcionalidade foi projetada com eficiência em mente, e o tempo de execução é calculado
- Além de demonstrar o tempo em notação Big O.

---

## 🛠️ Estrutura do Projeto

### Arquivos Principais:
- **main.cpp**: Contém a lógica principal do programa.

### Estrutura de Dados:
#### Lista Duplamente Encadeada
- Representa as páginas abertas no navegador.
- Cada nó contém:
  - **Título** e **URL** da página.
  - Ponteiros para os **nós anterior e próximo**.

#### Pilha
- Armazena o histórico de páginas removidas.
- Segue o modelo LIFO (*Last In, First Out*).

#### Fila
- Gerencia os favoritos em ordem de adição.
- Segue o modelo FIFO (*First In, First Out*).

---

## 🚀 Como Executar

### Pré-requisitos
- **Sistema Operacional:** Windows
- **Compilador:** Visual C++ (Microsoft Visual Studio)
- **Biblioteca:** `windows.h`

### Passos
1. Clone o repositório:
```bash
  git clone https://github.com/GabrielR0cha/Simulador_Navegador.git
  cd Simulador_Navegador
```
      
2. Compile o programa:
```bash
  g++ main.cpp -o navegador
```

3. Execute o programa:
```bash
  ./navegador
```
---

