/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Eric Torrontera Ruiz i Joan Fornis                        */
/*    Created:      26 2023                                                   */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Vision2              vision        2               
// Distance3            distance      3               
// Optical4             optical       4               
// Motor19              motor         19              
// Motor20              motor         20              
// Motor11              motor         11              
// Inertial1            inertial      1               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

/*###########################################################
#                   DEFINICIONES                            #
############################################################*/

/*###########################################################
#                     VARIABLES                             #
############################################################*/

int centro;
bool capturado = false;
bool flag = true;
int g = 0;
int col;

/*###########################################################
#               DECLARACIÓN DE FUNCIONES                    #
############################################################*/

void GirI();
void GirD();
void atras();
void adelante();
void para();
void capturar();
void liberar();
bool DetectorCubTaronja();
void buscar();
void corregirPos();

/*###########################################################
#                   PROGRAMA PRNCIPAL                       #
############################################################*/

int main() 
{
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  // Calibrar sensor incercial a pos (000) XYZ
  Inertial1.calibrate(); 
  wait(3,sec);

  // Si no detecta el cubo 
  while(!DetectorCubTaronja())
  {
    buscar();
  }
  
  // Mientras no haya capturado el cubo
  while(!capturado)
  {  
    // Si detecta el cubo
    if(DetectorCubTaronja())
    {
      // Si el objeto esta en el rango de captura
      if( 70 >= Distance3.objectDistance(mm)) 
      {
        para();
        capturar();
        capturado = true;
      }
      else if(centro < 75) // Si el cubo esta a la derecha
      {
        GirI();
      }
      else if(centro > 130) // Si el cubo esta a la izquierda
      {
        GirD();
      }
      else // Si el cubo esta en el rango central
      {
        adelante();
      }
    }
    else // Si la cam no ve el cubo por un instante (la cam es muy mala)
    {
      para();
    }
  } 
  
  // Iniciamos secuancia para volver a la base
  corregirPos();
  atras();
  wait(1,sec);
  para();
  liberar();
}

/*###########################################################
#                     CONTROLES ROBOT                       #
############################################################*/

void GirD()
{
  Motor19.setVelocity (10, percent);
  Motor20.setVelocity (10, percent);
  Motor19.spin(reverse);
  Motor20.spin(forward);
}

void GirI()
{
  Motor19.setVelocity (10, percent);
  Motor20.setVelocity (10, percent);
  Motor19.spin(forward);
  Motor20.spin(reverse);
}

void adelante()
{
  Motor19.setVelocity (30, percent);
  Motor20.setVelocity (30, percent);
  Motor19.spin(forward);
  Motor20.spin(forward);
}

void atras()
{
  Motor19.setVelocity (100, percent);
  Motor20.setVelocity (100, percent);
  Motor19.spin(reverse);
  Motor20.spin(reverse);
}

void para()
{
  Motor19.stop();
  Motor20.stop();
}

void capturar()
{
  Motor11.spin(forward);
  wait(0.5,sec);
  Motor11.stop();
}

void liberar()
{
  Motor11.spin(reverse);
  wait(0.5,sec);
  Motor11.stop();
}

/*###########################################################
#               OTRAS FUNCIONES Y METODOS                   #
############################################################*/

bool DetectorCubTaronja() 
{
  // Imprimeix en la pantalla VEX les dades obtingudes pels sensors 
  Brain.Screen.setFont(mono40);
  Brain.Screen.clearLine(1, black);
  Brain.Screen.setCursor(Brain.Screen.row(), 1);
  Vision2.takeSnapshot(Vision2__CUB_TARONJA);
  Brain.Screen.setCursor(2,1);
  Brain.Screen.print("Pos CN %d", centro); // Posició
  Brain.Screen.setCursor(1, 1);

  // Imprime en la pantalla del VEX si ha detectado o no el cubo de color naranja 
  if (Vision2.objectCount > 0) 
  {
    centro = Vision2.objects[0].centerX; // Si lo detecta una sola vez guardarà el valor de la ultima vez que se vió.
    Brain.Screen.print("Cubo naranja detectado");
    return true;
  } 
  else 
  {
    Brain.Screen.print("Cubo naranja not found");
    centro = 100;
    return false;
  }
}

void buscar()
{
  // El giroscopio desde su cento 0 hacia la izquierda va de 0 a 360 y desde su centro a la derecha 360 a 0
  if(Inertial1.heading(degrees) > 180)
  {
    g = Inertial1.heading(degrees) - 360; // 0 a -180
  }
  else
  {
    g = Inertial1.heading(degrees); // 0 a 360
  }

  // Busca en el lado derecho
  if(g < 45 && flag)
  {
    GirD();
  }
  else
  {
    flag = false; // Ping Pong (1)
  }

  // Busca en el lado izquierdo
  if((g > -45) && !flag)
  {
    GirI();
  }
  else
  {
    flag = true; // Ping Pong (2)
  }
}

void corregirPos()
{
  // Mientras el robot no este centrado
  while(Inertial1.heading(degrees) > 1)
  {
    // El giroscopio desde su cento 0 hacia la izquierda va de 0 a 360 y desde su centro a la derecha 360 a 0
    if(Inertial1.heading(degrees) > 180)
    {
      g = Inertial1.heading(degrees) - 360; // (P) Propongo angulo negativo para la derecha
    }
    else
    {
      g = Inertial1.heading(degrees);
    }

    //  Corregir angulo
    if(g > 0)
    {
      GirI();
    }
    if((g < 0))
    {
      GirD();
    }
  }
}

// pues la idea era que retornara un valor entre 0 y 4 para indicar al main que color tiene que buscar
int Elegir_Cubo()
{
  Optical4.setLightPower(100, percent);
  Optical4.setLight(ledState::on); 
  col = Optical4.color();
  
  if(col > 8 && col < 22) //Color taronja 
  {
    return 1;
  }
  else 
  {
    return 0;
  }
  
}

/* Hasta aquí llegue, entonces falta tomar el color del cubo antes de la comp
para que el robot sepa que cubo tiene que elegir y podemos hacer copy paste del pid del 
seguidor de linea anterior y ponerselo a este pero con objetivo un intervalo*/