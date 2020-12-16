# EECdspic
EEC analyzer managed by a dspic

Se pretende adquirir, almacenar, enviar y procesar una señal de electrocardiografía ECG.
La secuencia de acciones realizadas por el microcontrolador, grosso modo, será la siguiente:

1.	La señal proveniente de un amplificador de instrumentación monitoriza una señal de ECG la cual será muestreada y adquirida por el convertidor ADC del microcontrolador. En el arranque el microcontrolador indica un mensaje de bienvenida en el display LCD y espera la pulsación del botón OK para iniciar la adquisición.

2.	Los datos adquiridos son enviados, vía BT, a un PC para su representación gráfica (monitor) a una tasa de 50 envíos por segundo.

3.	Simultáneamente, dichos datos son almacenados en un fichero ubicado en la tarjeta de memoria microSD.

4.	En tiempo real se procesará la señal, mediante un algoritmo conocido, que tiene por objeto la determinación de la frecuencia cardíaca, la cual se presentará en el display LCD.

5.	Se indicará de manera acústica (bip) el ritmo cardiaco y, además, se indicara mediante una señal acústica continua de alarma, cuando dicha frecuencia es muy baja (paro cardíaco o situación de bradicardia <35ppm) o cuando se haya soltado un electrodo.

6.	Al pulsar el botón OK cesa la adquisición, el envío de los datos y se cierra el fichero de datos.
