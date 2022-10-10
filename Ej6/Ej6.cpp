/*
La ciudad de Buenos Aires requiere un estudio estadístico de infracciones realizadas por los vehículos en sus calles. 
Para ello cuenta con un archivo de infracciones “Infracciones.dat”, ordenado por fecha de la infracción creciente, con el siguiente diseño:  
    Patente (7 caracteres) , Fecha de Infracción (int, aammdd) , Código de infracción (int, 1 a 100)   
Se desea obtener un listado ordenado por código de infracción decreciente con las  patentes que cometieron más de una vez la misma infracción.
*/

#include <iostream>
#include <stdio.h>

using namespace std;

struct InfraccionesDelArchivo
{
    int Patente,FechaDeInfraccion,CodDeInfraccion;
};

struct Vehiculo
{
    int Patente;
};

struct NodoSub
{
    Vehiculo Info;
    NodoSub *Sgte;
};

struct Infracciones
{
    int CodDeInfraccion;
    NodoSub *ListaDePatente;
};

struct Nodo
{
    Infracciones Info;
    Nodo *Sgte;
    Nodo *Ant;
};

void CargaDeDatos(FILE*ArchivoA,Nodo*&Primero,Nodo*&Ultimo);
void InsertarPatente(NodoSub*&ListaDePatente,Vehiculo Dato);
void MostrarListado(Nodo*Lista);
Nodo *BuscarInsertar(Nodo*&Primero,Nodo*&Ultimo,Infracciones Dato);

int main()
{
    Nodo *Primero = NULL,*Ultimo = NULL;
    FILE *ArchivoA = fopen("Infracciones.dat","rb");

    if(ArchivoA == NULL)
    {
        cout << "ERROR" << endl;
    }
    else
    {
        CargaDeDatos(ArchivoA,Primero,Ultimo);

        cout << "Listado ordenado por codigo de infraccion con las patentes que cometieron mas de una vez la misma infraccion: " << endl;

        MostrarListado(Ultimo);
    }

    return 0;
}

void CargaDeDatos(FILE*ArchivoA,Nodo*&Primero,Nodo*&Ultimo)
{
    Nodo *Aux;
    InfraccionesDelArchivo I;
    Infracciones AuxI;
    Vehiculo V;

    fread(&I,sizeof(InfraccionesDelArchivo),1,ArchivoA);

    while(!feof(ArchivoA))
    {
        AuxI.CodDeInfraccion = I.CodDeInfraccion;
        V.Patente = I.Patente;
        AuxI.ListaDePatente = NULL;
        Aux = BuscarInsertar(Primero,Ultimo,AuxI);

        InsertarPatente(Aux->Info.ListaDePatente,V);

        fread(&I,sizeof(InfraccionesDelArchivo),1,ArchivoA);
    }
}

void InsertarPatente(NodoSub*&ListaDePatente,Vehiculo Dato)
{
    NodoSub *Nuevo,*Antecesor,*Aux;
    Nuevo = new NodoSub;
    Nuevo->Info = Dato;
    Aux = ListaDePatente;

    while(Aux != NULL && Aux->Info.Patente < Dato.Patente)
    {
        Antecesor = Aux;
        Aux = Aux->Sgte;
    }

    Nuevo->Sgte = Aux;

    if(Aux != ListaDePatente)
    {
        Antecesor->Sgte = Nuevo;
    }
    else
    {
        ListaDePatente = Nuevo;
    }
}

void MostrarListado(Nodo*Ultimo)
{
    int Repetidos = 0;
    Nodo *Aux = Ultimo;
    NodoSub *Actual,*AuxSub;

    while(Aux != NULL)
    {

        cout << "Codigo infraccion: " << Aux->Info.CodDeInfraccion << endl;

        Actual = Aux->Info.ListaDePatente;

        while(Actual != NULL)
        {

            AuxSub = Actual;

            do
            {
                AuxSub = AuxSub->Sgte;
                Repetidos++;
            }
            while(AuxSub != NULL && Actual->Info.Patente == AuxSub->Info.Patente);

            if(Repetidos >= 2)
            {
                cout << "Patente: " << Actual->Info.Patente << " - " << "Cantidad de veces cometidas: " << Repetidos << endl;
            }

            Repetidos = 0;
            Actual = AuxSub;
        }

        cout << "---------------------------------" << endl;

        Aux = Aux->Ant;
    }
}

Nodo *BuscarInsertar(Nodo*&Primero,Nodo*&Ultimo,Infracciones Dato)
{
    Nodo *Aux,*Antecesor;
    Aux = Primero;

    while(Aux != NULL && Aux->Info.CodDeInfraccion < Dato.CodDeInfraccion)
    {
        Aux = Aux->Sgte;
    }

    if(Aux != NULL && Dato.CodDeInfraccion == Aux->Info.CodDeInfraccion)
    {
        return Aux;
    }
    else
    {
        Nodo *Nuevo = new Nodo;
        Nuevo->Info = Dato;
        Nuevo->Sgte = Aux;

        if(Aux != Primero)
        {
            if(Aux != NULL)
            {
                Antecesor = Aux->Ant;
                Antecesor->Sgte = Nuevo;
                Aux->Ant = Nuevo;
            }
            else
            {
                Ultimo->Sgte = Nuevo;
                Nuevo->Ant = Ultimo;
                Ultimo = Nuevo;
            }
        }
        else
        {
            if(Primero != NULL)
            {
                Primero->Ant = Nuevo;
            }
            else
            {
                Ultimo = Nuevo;
            }

            Nuevo->Ant = NULL;
            Primero = Nuevo;
        }

        return Nuevo;
    }
}

