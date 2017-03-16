//ESPECTROMETRO Sergio Alvare Pelaez, Avelino Villa Pasarin


/*******************************************************************************
        Samples\TcpIp\RabbitWeb\thermo_graph.c
        Digi International Inc., 2010


		  Most of the interesting part of this sample is in the ZHTML file,
		  which includes JavaScript code to manipulate graphical objects.
		  The JS code is in graph.js.  See the two #ximport statements
		  below.

		  NOTE: since this uses the <canvas> HTML tag, including text
		  manipulations, you will need to use a modern browser such as
		  Firefox 3.5, Chrome or Safari.  Microsoft IE 8 is not supported as
		  of the date of writing, however IE 9 is expected to work.  Any browser
		  which supports HTML 5 should work.

		  See:
        samples\tcpip\rabbitweb\pages\thermo_graph.zhtml
        samples\tcpip\rabbitweb\pages\graph.js
        https://developer.mozilla.org/en/HTML/Canvas


*******************************************************************************/


#define TCPCONFIG 1



/*
 * This is needed to be able to use the RabbitWeb HTTP enhancements and the
 * ZHTML scripting language.
 */
#define USE_RABBITWEB 1

#memmap xmem

#use "dcrtcp.lib"
#use "http.lib"

/*
 * This page contains the ZHTML portion of the selection variable demonstration
 */
#ximport "espectroWeb_graph.zhtml"	espectroWeb_graph_zhtml
#ximport "samples/tcpip/rabbitweb/pages/graph.js"	graph_js

/* The default mime type for '/' must be first */
SSPEC_MIMETABLE_START
   // This handler enables the ZHTML parser to be used on ZHTML files...
	SSPEC_MIME_FUNC(".zhtml", "text/html", zhtml_handler),
	SSPEC_MIME(".html", "text/html"),
	SSPEC_MIME(".js", "text/javascript")
SSPEC_MIMETABLE_END

/* Associate the #ximported files with the web server */
SSPEC_RESOURCETABLE_START
	SSPEC_RESOURCE_XMEMFILE("/", espectroWeb_graph_zhtml),
	SSPEC_RESOURCE_XMEMFILE("/index.zhtml", espectroWeb_graph_zhtml),
	SSPEC_RESOURCE_XMEMFILE("/graph.js", graph_js)
SSPEC_RESOURCETABLE_END



#define SPI_SER_C

//DEFINIR QUE ES POR EL PUERTO C EN EL CONVERSOR A/D

#use "spi.lib"




#define tamanio 32
								// 80*36 * 15 sec = 12 hours
struct espectro_struct {
	// Basic data collection rate is one sample every 15 sec, however this
	// data is decimated (1:48) for the 12 hour history.  This data is
	// read-only to the form.
	int puntosEspectro[tamanio];				

	// Following vars are read/write and are controllable via a web form.
	int cota_inferior;				// Lower Y bound for graphical display
	int cota_superior;				// Upper Y bound for graphical display
	///////int interval;			// INTERVAL_20min, INTERVAL_12h
	///////int displ_units;		// UNITS_C, UNITS_F : display units
								//  (this struct data always in deg F)
	///////int curr_setpoint;	// Current setpoint
	///////int enabled;			// Whether thermostat enabled
};

// This is web registered for display
struct espectro_struct espectroWeb





	int ADleercanal( char canal){

          char escrito[3],leido[3];
          unsigned int i,j;


          escrito[0]=1;
          escrito[2]=0;

          if(canal==0){//genero el segundo byte a enviar
             escrito[1]=160;

           }

          else if(canal==1){//genero el segundo byte a enviar

            escrito[1]=224;

          }

       	BitWrPortI(PBDR,&PBDRShadow,0,5);//ectivo el CS

         for(i=0;i<10;i++);
         SPIWrRd(escrito,leido,3);
         for(i=0;i<10;i++);
         BitWrPortI(PBDR,&PBDRShadow,1,5);//desactivar en chip select


         return((leido[1]&&0x0F)*256+leido[2]);
   }


void transfer_display()
{
	// The actual data needs to be transferred to a #web registered variable
	// for transmission to the browser.  Pick the appropriately selected
	// data interval...  (Only the data is copied, the option settings
	// already live in the #web variable).
	
		memcpy(espectroWeb.puntosEspectro, modulos,
				sizeof(modulos));
}


      void main(){

              BitWrPortI(PBDDR,&PBDDRShadow,1,5);

             int ADleercanal();
	transfer_display();
             int valor;
            SPIinit();


			//inicializamos el vector para guardar las 64 muestras
			int x[64];

      	while(1){
                   costate{

						for(i=0;i<64;i++)
						{//se toman 64 muestras

                        x[i]=ADleercanal(0);
						//printf("el valor leido es %d\n",valor);

            			waitfor(DelayMs(1000));
                         }

						 fftreal(x,64,&blockexp);

						 //calculo los modulos
						 for(i=0;i<32;i++)
						 {

                    	modulos[i]=sqrt(x[2*i]*x[2*i]*1.0+x[2*i+1]*x[2*i+1]*1.0)

                     //multiplicar por la potencia de dos adecuada:
							modulos[i]=modulos[i]*pow2(5-blockexp);
                     //ajuste...
						 }


			/////PARA PRUEBAS

for(i=0;i<32;i++)
{
	modulos[i]=i*20;
}
///////////////////////

						 for(i=4;i<13;i++){

								if(modulos[i]>100){

									//alarma
                           BitWrPortI(PADR,&PADRShadow,0x01,2);
                           waitfor(DelaySec(1));
                           BitWrPortI(PADR,&PADRShadow,0x00,2);
								}
						 }

transfer_display();




         }

  
      }