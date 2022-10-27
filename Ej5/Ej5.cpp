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

struct Cliente
{
    int CodDeCliente;
};

struct Nodo
{
    Cliente Info;
    Cuotas[12];
    Nodo *Sgte;
};

void CargaDeDatos(FILE *ArchivoA,Nodo *&Lista);
void BuscarInsertar(Nodo *&Lista,Cliente Dato,int NroDeCuota);
void MostrarListadoDeDeudores(Nodo *Lista);

int main()
{
    FILE *ArchivoA = fopen("Cobranzas.dat","rb");

    if(ArchivoA == NULL)
    {
        cout << "ERROR" << endl;
    }
    else
    {
        Nodo *Lista = NULL;

        CargaDeDatos(ArchivoA,Lista);

        cout << "Listado de aquellos que adeudan cuotas: " << endl;

        MostrarListadoDeDeudores(Lista);

        cout << "---------------------------------" << endl;
    }

    return 0;
}

void CargaDeDatos(FILE *ArchivoA,Nodo *&Lista)
{
    CobranzasDelArchivo C;
    Cliente AuxCliente;
    Nodo *Aux;

    fread(&C,sizeof(CobranzasDelArchivo),1,ArchivoA);

    while(!feof(ArchivoA))
    {
        AuxCliente.CodDeCliente = C.CodDeCliente;
        Aux = BuscarInsertar(Lista,AuxCliente,C.NroDeCuota);

        fread(&C,sizeof(CobranzasDelArchivo),1,ArchivoA);
    }
}

void BuscarInsertar(Nodo *&Lista,Cliente Dato,int NroDeCuota)
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
        Aux->Cuotas[NroDeCuota - 1] = true;
    }
    else
    {
        Nodo *Nuevo = new Nodo;
        Nuevo->Info = Dato;
        Nuevo->Sgte = Aux;

        for(int i = 0; i < 11; i++)
        {
            if(NroDeCuota == i + 1)
            {
                Nuevo->Cuotas[i] = true;
            }
            else
            {
                Nuevo->Cuotas[i] = false;
            }
        }

        if(Aux != Lista)
        {
            Antecesor->Sgte = Nuevo;
        }
        else
        {
            Lista = Nuevo;
        }
    }
}

void MostrarListadoDeDeudores(Nodo *Lista)
{
    Nodo *Aux;

    while(Aux != NULL)
    {
        cout << "Codigo de cliente: " << Aux->Info.CodDeCliente << endl;

        for(int i = 0; i < 11; i++)
        {
            if(Aux->Cuotas[i] != true)
            {
                cout << "Nro de cuota: " << i + 1 << endl;
            }
        }

        Aux = Aux->Sgte;
    }
}
