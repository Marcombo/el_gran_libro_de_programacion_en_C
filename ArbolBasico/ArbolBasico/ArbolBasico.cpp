// ArbolBasico.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>
#include <ctype.h>

#pragma warning(disable:4996)

typedef struct tagBaseCell * BasePtr;

typedef struct tagBaseCell    // Nodo básico
{                          		           
	char    szClave[100];  // Clave original de ta tupla clave-valor
	int     nUtil;         // Valor
	BasePtr pParent;
	BasePtr	pLeft;		   // Izqda para la rama "<"
	BasePtr	pRight;        // Dcha para la rama ">"
};

//Variable global de la raíz del árbol
BasePtr _pRaizArbol;

//Variable global para contador de elementos:
static int _nContadorElementos = 0;

BasePtr PrepararArbol()
{
	_pRaizArbol = (BasePtr)malloc(sizeof(tagBaseCell));
	if (_pRaizArbol)
	{
		memset(_pRaizArbol, 0, sizeof(tagBaseCell));
		// Estas tres instrucciones no son necesarias, 
		// porque el memset ya ha puesto a cero toda
		// la estructura del nodo, pero nosotros las
		// hacemos explícitas así:
		_pRaizArbol->pParent = NULL;
		_pRaizArbol->pLeft = NULL;
		_pRaizArbol->pRight = NULL;
	}
	return _pRaizArbol;
}

BasePtr InsertarClaveValor(BasePtr pParent, const char* lpszClave, int nRegistro)
{
	int nError = 0;
	int nComparacion = 0;
	BasePtr pNodo = NULL;
	BasePtr pNuevoNodo = NULL;

	if (!pParent)
	{
		//No se ha inicializado la raíz de este sub-árbol:
		pNuevoNodo = (BasePtr)malloc(sizeof(tagBaseCell));
		memset(pNuevoNodo, 0, sizeof(tagBaseCell));
		strcpy(pNuevoNodo->szClave, lpszClave);
		pNuevoNodo->nUtil = nRegistro;
		pNuevoNodo->pLeft = NULL;
		pNuevoNodo->pRight = NULL;
		return pNuevoNodo;
	}

	if (strlen(pParent->szClave) == 0)
	{
		//Existe una raiz de sub-árbol, pero está vacía
		memset(pParent, 0, sizeof(tagBaseCell));
		strcpy(pParent->szClave, lpszClave);
		pParent->nUtil = nRegistro;
		pParent->pLeft = NULL;
		pParent->pRight = NULL;
		pParent->pParent = NULL;
		return pParent;
	}
	else
	{
		nComparacion = strcmp(lpszClave, pParent->szClave);
		if (nComparacion < 0)
		{
			pNodo = pParent->pLeft;
			pNuevoNodo = InsertarClaveValor(pNodo, lpszClave, nRegistro);
			if (pNuevoNodo)
			{
				if (!pNuevoNodo->pParent)
				{
					if (pNuevoNodo != pParent)
					{
						pParent->pLeft = pNuevoNodo;
						pNuevoNodo->pParent = pParent;
					}
				}
			}
		}
		else
		{
			if (nComparacion > 0)
			{
				pNodo = pParent->pRight;
				pNuevoNodo =  InsertarClaveValor(pNodo, lpszClave, nRegistro);
				if (pNuevoNodo)
				{
					if (!pNuevoNodo->pParent)
					{
						if (pNuevoNodo != pParent)
						{
							pParent->pRight = pNuevoNodo;
							pNuevoNodo->pParent = pParent;
						}
					}
				}
			}
		}
	}
	return pNuevoNodo;
}

bool VerificarSiEsIgual(BasePtr pNodoA, BasePtr pNodoB, const char * lpszExpresion)
{
	
	int nLen = 0;

	bool bIguales = pNodoA == pNodoB;

	if (lpszExpresion)
	{
		nLen = (int)strlen(lpszExpresion);
		if (nLen)
		{
			printf(lpszExpresion, (bIguales) ? "SI" : "NO");
		}
	}
	return bIguales;
}

BasePtr BuscarNodo(BasePtr pParent,  char* lpszClave, int* pnNumComparaciones)
{
	BasePtr pFoundNode = NULL;
	int nComparacion = 0;

	if (pParent)
	{
		nComparacion = strcmp(lpszClave, pParent->szClave);
		*pnNumComparaciones = *pnNumComparaciones + 1; //Incrementamos contador de comparaciones
		if (nComparacion == 0) //Son iguales: lo hemos encontrado
		{
			return pParent;
		}
		if (nComparacion < 0) //Es más pequeño alfabéticamente:
		{
			pFoundNode = BuscarNodo(pParent->pLeft, lpszClave, pnNumComparaciones);
		}
		else //Es más grande alfabéticamente:
		{
			pFoundNode = BuscarNodo(pParent->pRight, lpszClave, pnNumComparaciones);
		}
	}
	return pFoundNode;
}

int HacerAlgo(BasePtr pParent)
{
	// Función vacía a efectos de prototipado
	// o función que llama a otra función
	int nResultado = 0;

	//Hacer algo

	return nResultado;
}

int ImprimirNodo(BasePtr pParent)
{
	printf("Clave = %s\tRegistro = %d\n", 
		   pParent->szClave, 
		   pParent->nUtil);

	return 0;
}

int ContarElemento(BasePtr pParent)
{
	_nContadorElementos++;

	return _nContadorElementos;
}

int LiberarNodo(BasePtr pParent)
{
	if (pParent)
	{
		printf("Liberando %s\t-> ", pParent->szClave);
		// Si se produjera un error al eliminar un 
		// puntero inválido, fallaría en el free
		// y no llegaría nunca a imprimir el OK
		free(pParent);
		pParent = NULL;
		printf("OK.\n"); //<--nos referimos a este OK
	}
	return 0;
}

typedef int (* LPFNHACERALGO)(BasePtr pParent);

void Traversa_In_Order(BasePtr pParent, LPFNHACERALGO lpfnHacerAlgo)
{
	if (pParent == NULL)
	{
		return;
	}
	//Atravesar por la izquierda:
	if (pParent->pLeft != NULL)
	{
		Traversa_In_Order(pParent->pLeft, lpfnHacerAlgo);
	}
	//Acción sobre nodo actual:
	//printf("%s\t%d\n", pParent->szClave, pParent->nUtil);
	int nResultado = lpfnHacerAlgo(pParent);

	//Atravesar por la derecha:
	if (pParent->pRight)
	{
		Traversa_In_Order(pParent->pRight, lpfnHacerAlgo);
	}
}

void Traversa_Post_Order(BasePtr pParent, LPFNHACERALGO lpfnHacerAlgo)
{
	if (pParent == NULL)
	{
		return;
	}
	//Atravesar por la izquierda:
	if (pParent->pLeft != NULL)
	{
		Traversa_Post_Order(pParent->pLeft, lpfnHacerAlgo);
	}

	//Atravesar por la derecha:
	if (pParent->pRight)
	{
		Traversa_Post_Order(pParent->pRight, lpfnHacerAlgo);
	}

	//Acción sobre nodo actual:
	//printf("%s\t%d\n", pParent->szClave, pParent->nUtil);
	int nResultado = lpfnHacerAlgo(pParent);
}

/***********************************************
*                                              *
*             PROGRAMA PRINCIPAL               *
*                                              *
************************************************/

int main()
{
	// Damos de alta la raíz del árbol:
	_pRaizArbol = NULL;
	bool bOK = false;
	char szClaveBuscada[100];
	char Ch = 'S';
	int nRegistro = 0;
	int nNumComparaciones = 0;

	// Estos punteros son para hacer depuración y asegurarnos 
	// que el puntero que nos retornará la función BuscarNodo
	// es el mismo que tenía el nodo encontrado cuando se 
	// generó mediante malloc.
	BasePtr pNodoCreadoBenito  = NULL;
	BasePtr pNodoCreadoAna     = NULL;
	BasePtr pNodoCreadoCarlos  = NULL;
	BasePtr pNodoCreadoAbel    = NULL;
	BasePtr pNodoCreadoArmando = NULL;
	BasePtr pNodoCreadoCamila  = NULL;
	BasePtr pNodoCreadoDomingo = NULL;
	BasePtr pNodoEncontrado = NULL;

	_pRaizArbol = PrepararArbol();
	if (_pRaizArbol != NULL)
	{
		// Crear nodos y tomar nota del puntero 
		// donde se ha creado el nodo para poder 
		// comparar después con el puntero 
		// encontrado mediante la función BuscarNodo
		pNodoCreadoBenito  = InsertarClaveValor(_pRaizArbol, "Benito",  1);
		pNodoCreadoAna     = InsertarClaveValor(_pRaizArbol, "Ana",     2);
		pNodoCreadoCarlos  = InsertarClaveValor(_pRaizArbol, "Carlos",  3);
		pNodoCreadoAbel    = InsertarClaveValor(_pRaizArbol, "Abel",    4);
		pNodoCreadoArmando = InsertarClaveValor(_pRaizArbol, "Armando", 5);
		pNodoCreadoDomingo = InsertarClaveValor(_pRaizArbol, "Camila",  6);
		pNodoCreadoDomingo = InsertarClaveValor(_pRaizArbol, "Domingo", 7);

		//Mecanismo de búsqueda:
		Ch = 'S';
		while (Ch == 'S')
		{
			printf("Entre la clave que quiere buscar:");
			scanf("%s", szClaveBuscada);
			Ch = getchar();

			nNumComparaciones = 0;
			pNodoEncontrado = BuscarNodo(_pRaizArbol, szClaveBuscada, &nNumComparaciones);
			if (pNodoEncontrado)
			{
				nRegistro = pNodoEncontrado->nUtil;
				printf("Hemos encontrado a %s en el registro %d ", szClaveBuscada, nRegistro);
				printf("mediante %d comparaciones.\n", nNumComparaciones);
			}
			else
			{
				printf("No hemos encontrado a %s\n", szClaveBuscada);
			}
			printf("Desea seguir buscando? (S/N)");
			
			Ch = _toupper(getchar());	
		}
	}

	// Declaración de variable del tipo puntero a función
	LPFNHACERALGO lpfnHacerAlgo = NULL;

	// Traversa in order (listado):
	// Asignación del puntero a función 
	// para que apunte a la función ImprimirNodo:
	lpfnHacerAlgo = ImprimirNodo;

	Traversa_In_Order(_pRaizArbol, lpfnHacerAlgo);

	//Contar elementos del arbol:
	lpfnHacerAlgo = ContarElemento;
	Traversa_In_Order(_pRaizArbol, lpfnHacerAlgo);
	printf("\nHemos contado %d elementos en el arbol.\n", _nContadorElementos);

	// Liberar nodos del árbol: aquí recurrimos 
	// a otra traversa diferente:
	// Se trata de la Traversa Post Order:
	lpfnHacerAlgo = LiberarNodo;
	Traversa_Post_Order(_pRaizArbol, lpfnHacerAlgo);

	printf("Fin de programa.\n");
}

