#include <Windows.h> 
#include <locale.h>
#include <chrono>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <iomanip>

//Representa o tamanho máximo da fila
#define MAX_SIZE 1000 

//Struct que representa a página
typedef struct Pagina {
    int valor;
    std::wstring nome;
    Pagina* prox;
    Pagina* ant;
} Pagina;

//Struct que representa a Fila dos favoritos
typedef struct FilaFavoritos {
    Pagina* paginas[MAX_SIZE];
    int inicio;
    int fim;
} FilaFavoritos;

//Struct que representa a Pilha das páginas fechadas
typedef struct PilhaFechados {
    std::wstring pagina;
    PilhaFechados* prox;
} PilhaFechados;

//Struct que representa a pilha, salva o início e fim
typedef struct Pilha {
    PilhaFechados* inicio;
    PilhaFechados* fim;
} Pilha;

//Função para mudar a cor do terminal
void SetColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

//Verifica se a pilha está vazia
int VazioPilha(Pilha* pilha) {
    return pilha->inicio == NULL;
}

//Ele cria um novo nodo pilha
Pilha* CriarPilha() {
    Pilha* pilha = new Pilha;
    pilha->inicio = NULL;
    pilha->fim = NULL;
    return pilha;
}

//Cria um nodo que vai na pilha
PilhaFechados* CriarNodoPilha(Pagina* pagina) {
    PilhaFechados* newNode = new PilhaFechados;

    newNode->pagina = pagina->nome;
    newNode->prox = NULL;

    return newNode;
}

//Adicionan na pilha
void AdicionaPilha(Pilha* pilha, Pagina* pagina) {
    PilhaFechados* novoNodo = CriarNodoPilha(pagina);
    if (VazioPilha(pilha)) {
        pilha->inicio = pilha->fim = novoNodo;
    }
    else {
        novoNodo->prox = pilha->inicio;
        pilha->inicio = novoNodo;
    }
}

//Remove da pilha
std::wstring RemoverPilha(Pilha* pilha) {
    if (VazioPilha(pilha)) {
        return L"";
    }
    PilhaFechados* temp = pilha->inicio;
    std::wstring removido = temp->pagina;
    pilha->inicio = pilha->inicio->prox;

    if (pilha->inicio == NULL) {
        pilha->fim = NULL;
    }

    delete temp;
    return removido;
}

//Limpa a pilha inteira
void LimpaPilha(Pilha* pilha) {
    while (!VazioPilha(pilha)) {
        PilhaFechados* temp = pilha->inicio;
        pilha->inicio = pilha->inicio->prox;

        if (pilha->inicio == NULL) {
            pilha->fim = NULL;
        }

        delete temp;
    }
    return;
}

//Cria nodo pagina
Pagina* CriaPagina(int valor) {
    Pagina* newNode = new Pagina;
    newNode->valor = valor;
    newNode->prox = NULL;
    newNode->ant = NULL;

    if (valor == 0) {
        newNode->nome = L"HOME";
        return newNode;
    }
    else {
        newNode->nome = L"Pagina " + std::to_wstring(valor);

        return newNode;
    }
}

//Inserir nodo pagina na lista
Pagina* InserirPaginaLista(Pagina* lista, int valor) {
    Pagina* novo = CriaPagina(valor);

    if (lista == NULL) {
        return novo;
    }

    Pagina* aux = lista;

    if (valor <= lista->valor) {
        novo->prox = lista;
        lista->ant = novo;
        return novo;
    }

    while (aux->prox != NULL && aux->prox->valor < valor) {
        aux = aux->prox;
    }

    novo->prox = aux->prox;
    novo->ant = aux;

    if (aux->prox != NULL) {
        aux->prox->ant = novo;
    }
    aux->prox = novo;

    return novo;
}

//Inserir nodo na pagina na lista com nome
Pagina* InserirPaginaListaComNome(Pagina* lista, std::wstring pagina, int valor) {

    Pagina* novo = CriaPagina(valor);
    novo->nome = pagina;

    if (lista == NULL) {
        return novo;
    }

    Pagina* aux = lista;

    if (valor <= lista->valor) {
        novo->prox = lista;
        lista->ant = novo;
        return novo;
    }

    while (aux->prox != NULL && aux->prox->valor < valor) {
        aux = aux->prox;
    }

    novo->prox = aux->prox;
    novo->ant = aux;

    if (aux->prox != NULL) {
        aux->prox->ant = novo;
    }
    aux->prox = novo;

    return novo;
}

//Remove o nodo pagina na lista
Pagina* RemovePaginaLista(Pagina* pagAtual) {
    if (pagAtual == NULL) {
        std::wcout << "\nEsse nó não existe";
        return NULL;
    }

    Pagina* paraRemover = pagAtual;

    if (paraRemover->ant != NULL) {
        paraRemover->ant->prox = paraRemover->prox;
    }

    if (paraRemover->prox != NULL) {
        paraRemover->prox->ant = paraRemover->ant;
    }

    Pagina* pageRetornado;

    if (paraRemover->ant != NULL) {
        pageRetornado = paraRemover->ant;
    }
    else {
        pageRetornado = paraRemover->prox;
    }

    delete paraRemover;

    return pageRetornado;
}

//Limpa lista de paginas
Pagina* LimparHistorico(Pagina* pageAtual) {
    Pagina* aux = pageAtual;

    while (aux->ant != NULL) {
        aux = aux->ant;
    }

    while (aux->prox != NULL) {
        aux = RemovePaginaLista(aux);

    }

    return aux;

}

// Verifica se a fila está vazia
int VaziaFila(FilaFavoritos* fila) {
    return (fila->inicio == -1 && fila->fim == -1);
}

// Verifica se a fila está cheia
int CheiaFila(FilaFavoritos* fila) {
    return (fila->fim + 1) % MAX_SIZE == fila->inicio;
}

// Inicializa a fila
FilaFavoritos* CriaFila() {
    FilaFavoritos* fila = new FilaFavoritos;
    fila->inicio = -1;
    fila->fim = -1;
    return fila;
}

// Adiciona um elemento no fim da fila
Pagina* InserirFila(FilaFavoritos* fila, Pagina* pagina) {
    Pagina* copiaPagina = new Pagina;

    if (CheiaFila(fila)) {
        std::wcout << "Erro: Fila Cheia\n";
        return NULL;
    }
    if (VaziaFila(fila)) {
        fila->inicio = 0;
    }
    fila->fim++;

    copiaPagina->nome = pagina->nome;
    copiaPagina->valor = fila->fim;

    fila->paginas[fila->fim] = copiaPagina;
    return pagina;
}

// Remove e retorna o elemento do início da fila
Pagina* RemoveFila(FilaFavoritos* fila) {
    if (VaziaFila(fila)) {
        return NULL;
    }
    Pagina* pagina = fila->paginas[fila->inicio];
    fila->inicio++;

    if (fila->inicio > fila->fim) {
        fila->inicio = -1;
        fila->fim = -1;
    }
    return pagina;
}

//Limpa a fila
void LimparFila(FilaFavoritos* fila) {
    int i = fila->inicio;
    do {
        RemoveFila(fila);
        i = (i + 1) % MAX_SIZE;
    } while (i != (fila->fim + 1) % MAX_SIZE);
    std::wcout << "|\n";
}

//Busca um nodo na fila
std::wstring BuscarNodoFila(FilaFavoritos* fila, int valor) {
    for (int i = fila->inicio; i <= fila->fim; i++) {
        if (valor == fila->paginas[i]->valor) {
            return fila->paginas[i]->nome;
        }
    }

    return L"";
}

//Imprimir Páginas
void ImprimirPaginas(Pagina* pageAtual) {
    Pagina* aux = pageAtual;

    if (aux == NULL) {
        SetColor(4);
        std::wcout << "NENHUMA PÁGINA ABERTA";
        SetColor(7);
        return;
    }

    while (aux->ant != NULL) {
        aux = aux->ant;
    }

    while (aux != NULL) {
        if (aux->valor == pageAtual->valor) {
            SetColor(10);
            std::wcout << "| =>> " << aux->nome << " <<= |";
            SetColor(7);
        }
        else {
            std::wcout << "| " << aux->nome << " |";
        }
        aux = aux->prox;
    }

};

//Imprimr header junto com o tempo
void ImprimirCabecalhoECalculaTempo(std::chrono::high_resolution_clock::time_point inicioExecucao,
    std::chrono::high_resolution_clock::time_point fimExecucao, std::wstring custo) {

    double tempo = std::chrono::duration<double, std::milli>(fimExecucao - inicioExecucao).count();
    std::wcout << "|============================= <> NAVEGADOR <> =============================|" << std::endl;
    std::wcout << "|___________________________________________________________________________|" << std::endl;
    std::wcout << "    Tempo de execução:" << std::fixed << std::setprecision(3) << tempo << "ms";
    std::wcout << "            Tempo em Big O: " << custo << "                   " << std::endl;

}

//Imprimir interface do navegador
void ImprimirInterfaceNavegador(Pagina* paginaAtual, FilaFavoritos* favoritos) {


    std::wcout << "|---------------------------------------------------------------------------|" << std::endl;
    std::wcout << "|        Navegação       | |        Favoritos      | |       Outros         |" << std::endl;
    std::wcout << "|<1> Abrir página        | |<7> Adicionar Favoritos| |<5>  Limpar Histórico |" << std::endl;
    std::wcout << "|<2> Pesquisar na Página | |<8> Remover Favoritos  | |<10> Recuperar página |" << std::endl;
    std::wcout << "|<3> Fechar página       | |<9> Listar Favoritos   | |<11> Ver Excluídos    |" << std::endl;
    std::wcout << "|                        | |<12> Buscar Favoritos  | |                      |" << std::endl;
    std::wcout << "|---------------------------------------------------------------------------|" << std::endl;
    std::wcout << "| <4> Navegar para a esquerda   |  <0> Sair   |  <6> Navegar para a Direita |" << std::endl;
    std::wcout << "|___________________________________________________________________________|\n" << std::endl;

    ImprimirPaginas(paginaAtual);

    SetColor(2);
    std::wcout << "\n\n >> ";
    SetColor(7);

};

//Imorimir pesquisar 
void ImprimirPesquisar() {
    SetColor(14);
    std::wcout << "\n\n|--------------------------------------------------------------------------| " << std::endl;
    std::wcout << "Busca: ";
    SetColor(7);
};

//Imprimir pilha dos fechados
void ImprimirFechados(Pilha* pilha) {
    if (VazioPilha(pilha)) {
        SetColor(4);
        std::wcout << "<>------<> Pilha de fechados vazio<>------<>\n" << std::endl;
        SetColor(7);
        return;
    }
    PilhaFechados* temp = pilha->inicio;

    SetColor(4);
    std::wcout << "<>------<> Fechados <>------<>" << std::endl;
    SetColor(7);
    while (temp != NULL) {

        SetColor(4);
        std::wcout << "=> ";
        SetColor(7);
        std::wcout << temp->pagina << "\n" << std::endl;
        temp = temp->prox;
    }
}

//Imprimir fila de favoritos
int ImprimirFila(FilaFavoritos* fila) {
    if (VaziaFila(fila)) {
        SetColor(9);
        std::wcout << "<>------<> Não existe favoritos cadastrados <>------<>" << std::endl;
        SetColor(7);
        return -1;
    }

    SetColor(9);
    std::wcout << "<>------<> Favoritos <>------<>" << std::endl;
    SetColor(7);

    for (int i = fila->inicio; i <= fila->fim; i -= -1) {
        SetColor(9);
        std::wcout << i << " => ";
        SetColor(7);
        std::wcout << fila->paginas[i]->nome << std::endl;

    }

    return 1;

}

int main()
{

    std::setlocale(LC_ALL, "en_US.UTF-8");
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    int numeroPagina = 1;
    int opcao, aux = 0;
    std::wstring busca, nomePagina;
    std::wstring custo = L"";

    std::chrono::high_resolution_clock::time_point inicioExecucao{};
    std::chrono::high_resolution_clock::time_point fimExecucao{};

    Pagina* paginaAtual = CriaPagina(0);

    FilaFavoritos* favoritos = CriaFila();

    Pilha* removidos = CriarPilha();

    do {

        system("cls");
        ImprimirCabecalhoECalculaTempo(inicioExecucao, fimExecucao, custo);
        ImprimirInterfaceNavegador(paginaAtual, favoritos);

        std::wcin >> opcao;
        std::wcin.ignore();

        switch (opcao) {
        case 1:

            inicioExecucao = std::chrono::high_resolution_clock::now();
            paginaAtual = InserirPaginaLista(paginaAtual, numeroPagina);
            fimExecucao = std::chrono::high_resolution_clock::now();
            custo = L"O(n)";

            numeroPagina++;

            break;
        case 2:
            ImprimirPesquisar();
            std::getline(std::wcin, busca);
            inicioExecucao = std::chrono::high_resolution_clock::now();
            paginaAtual->nome = busca;
            fimExecucao = std::chrono::high_resolution_clock::now();
            custo = L"O(1)";

            break;

        case 3:

            inicioExecucao = std::chrono::high_resolution_clock::now();
            AdicionaPilha(removidos, paginaAtual);
            paginaAtual = RemovePaginaLista(paginaAtual);
            fimExecucao = std::chrono::high_resolution_clock::now();
            custo = L"O(1)";

            break;

        case 6:
            inicioExecucao = std::chrono::high_resolution_clock::now();
            if (paginaAtual->prox != NULL)
                paginaAtual = paginaAtual->prox;
            fimExecucao = std::chrono::high_resolution_clock::now();
            custo = L"O(1)";

            break;

        case 4:
            inicioExecucao = std::chrono::high_resolution_clock::now();
            if (paginaAtual->ant != NULL)
                paginaAtual = paginaAtual->ant;
            fimExecucao = std::chrono::high_resolution_clock::now();
            custo = L"O(1)";

            break;

        case 5:
            inicioExecucao = std::chrono::high_resolution_clock::now();

            if (paginaAtual != NULL) {
                paginaAtual = LimparHistorico(paginaAtual);

                paginaAtual->ant = NULL;
                paginaAtual->prox = NULL;
                paginaAtual->valor = 0;
                paginaAtual->nome = L"HOME";
                numeroPagina = 1;

                LimparFila(favoritos);
                LimpaPilha(removidos);

            }
            fimExecucao = std::chrono::high_resolution_clock::now();
            custo = L"O(n + m + G)";
            break;

        case 7:
            inicioExecucao = std::chrono::high_resolution_clock::now();
            InserirFila(favoritos, paginaAtual);
            fimExecucao = std::chrono::high_resolution_clock::now();
            custo = L"O(1)";

            break;


        case 8:
            inicioExecucao = std::chrono::high_resolution_clock::now();
            AdicionaPilha(removidos, paginaAtual);
            RemoveFila(favoritos);
            fimExecucao = std::chrono::high_resolution_clock::now();
            custo = L"O(1)";
            break;

        case 9:
            inicioExecucao = std::chrono::high_resolution_clock::now();
            ImprimirFila(favoritos);
            fimExecucao = std::chrono::high_resolution_clock::now();
            std::wcin;
            std::wcin.ignore();
            break;
            custo = L"O(n)";

        case 10:
            inicioExecucao = std::chrono::high_resolution_clock::now();

            nomePagina = RemoverPilha(removidos);

            if (nomePagina != L"") {
                paginaAtual = InserirPaginaListaComNome(paginaAtual, nomePagina, numeroPagina);
                numeroPagina++;
            }
            fimExecucao = std::chrono::high_resolution_clock::now();
            custo = L"O(n)";

            break;
        case 11:
            inicioExecucao = std::chrono::high_resolution_clock::now();
            ImprimirFechados(removidos);
            fimExecucao = std::chrono::high_resolution_clock::now();
            custo = L"O(n)";
            std::wcin;
            std::wcin.ignore();

            break;

        case 12:
            aux = ImprimirFila(favoritos);

            if (aux < 0) {
                std::wcin;
                std::wcin.ignore();
                break;
            }

            std::wcin >> aux;
            std::wcin.ignore();

            inicioExecucao = std::chrono::high_resolution_clock::now();

            nomePagina = BuscarNodoFila(favoritos, aux);

            if (nomePagina != L"") {
                paginaAtual = InserirPaginaListaComNome(paginaAtual, nomePagina, numeroPagina);
                numeroPagina++;
            }
            custo = L"O(n + m)";
            fimExecucao = std::chrono::high_resolution_clock::now();

            break;
        case 0:
            std::wcout << "Saindo do programa..." << std::endl;
            break;
        };

        Sleep(100);

    } while (opcao != 0);


}

