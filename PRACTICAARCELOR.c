#define SPI_SER_C

//DEFINIR QUE ES POR EL PUERTO C EN EL CONVERSOR A/D

#use "spi.lib"


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
	
		memcpy(thermo.temp, thermo_20min.temp,
				sizeof(thermo.temp) * 5);
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

            			waitfor(DelayMs(5));
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

						 for(i=4;i<13;i++){

								if(modulos[i]>100){

									//alarma
                           BitWrPortI(PADR,&PADRShadow,0x01,2);
                           waitfor(DelaySec(1));
                           BitWrPortI(PADR,&PADRShadow,0x00,2);
								}
						 }
//////
transfer_display();




/////





         }

  
      }