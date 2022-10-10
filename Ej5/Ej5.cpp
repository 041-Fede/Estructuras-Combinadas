/*
Una compañía que ofrece préstamos a sus clientes a pagar en 12 meses, desea obtener un listado con los códigos de clientes y los números de las cuotas adeudadas de aquellos clientes que adeuden cuotas, ordenado por código de cliente. 
Para ello cuenta con un archivo de cobranzas, sin ningún orden, con el siguiente diseño:   
Código de cliente (int) , Número de cuota (1 a 12)
*/

#include <iostream>
#include <stdio.h>

using namespace std;

struct CobranzasDelArchivo
{
    int CodDeCliente,NroDeCuota;
};

struct Cuotas
{
    int NroDeCuota;
};

struct NodoSub
{
    Cuotas Info;
    NodoSub *Sgte;
};

struct Cliente
{
    int CodDeCliente;
    NodoSub *ListaDeCuotas;
};

struct Nodo
{
    Cliente Info;
    Nodo *Sgte;
};

void CargaDeDatos(FILE*ArchivoA,Nodo*&Lista);
void InsertaSubLista(NodoSub*&ListaDeCuotas,Cliente Dato);
Nodo *BuscarInsertar(Nodo*&Lista,Cliente Dato);

int main()
{
    Nodo *Lista = NULL;
    FILE *ArchivoA = fopen("Cobranzas.dat","rb");

    if(ArchivoA == NULL)
    {
        cout << "ERROR" << endl;
    }
    else
    {
        CargaDeDatos(ArchivoA,Lista);

        cout << "Listado de aquellos que adeudan cuotas: " << endl;

        MostrarListado(Lista);

        cout << "---------------------------------" << endl;
    }

    return 0;
}

void CargaDeDatos(FILE*ArchivoA,Nodo*&Lista)
{
    CobranzasDelArchivo C;
    Cliente AuxCliente;
    Cuotas AuxCoutas;
    Nodo *Aux;

    fread(&C,sizeof(CobranzasDelArchivo),1,ArchivoA);

    while(!feof(ArchivoA))
    {
        AuxCliente.CodDeCliente = C.CodDeCliente;
        AuxCoutas.NroDeCuota = C.NroDeCuota;
        AuxCliente.ListaDeCuotas = NULL;
        Aux = BuscarInsertar(Lista,AuxCliente);

        InsertarCuota(Aux->Info.ListaDeCuotas,AuxCoutas);

        fread(&C,sizeof(CobranzasDelArchivo),1,ArchivoA);
    }
}

void InsertaSubLista(NodoSub*&ListaDeCuotas,Cliente Dato)
{
    NodoSub *Nuevo,*Antecesor,*Aux;
    Nuevo = new NodoSub;
    Nuevo->Info = Dato;
    Aux = ListaDeCuotas;

    while(Aux != NULL && Aux->Info.NroDeCuota < Dato.NroDeCuota)
    {
        Antecesor = Aux;
        Aux = Aux->Sgte;
    }

    Nuevo->Sgte = Aux;

    if(Aux != ListaDeCuotas)
    {
        Antecesor->Sgte = Nuevo;
    }
    else
    {
        ListaDeCuotas = Nuevo;
    }
}

Nodo *BuscarInsertar(Nodo*&Lista,Cliente Dato)
{
    Nodo *Aux,*Antecesor;
    Aux = Lista;

    while(Aux != NULL && Aux->Info.CodDeCliente < Dato.CodDeCliente)
    {
        Antecesor = Aux;
        Aux = Aux->Sgte;
    }

    if(Aux != NULL && Dato.CodDeCliente == Aux->Info.CodDeCliente)
    {
        return Aux;
    }
    else
    {
        Nodo *Nuevo = new Nodo;
        Nuevo->Info = Dato;
        Nuevo->Sgte = Aux;

        if(Aux != Lista)
        {
            Antecesor->Sgte = Nuevo;
        }
        else
        {
            Lista = Nuevo;
        }

        return Nuevo;
    }
}
