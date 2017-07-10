//clear

//data = csvRead("C:\Users\erich\Dropbox\Arduino\Projekte\AccuChargerExtension\ChargingStateAlgorihm\messdaten_200mA.txt");

nrOfSamples = length(data)-4;

millisPerSample = ( data($) - data(1) ) / nrOfSamples

//plot(data(3:870000))
//plot(data($-800000:$-2))
plot(data(3:3:1800000))
xgrid

ex = 0.95;
dataFiltered = filter(1-ex, [1 -ex], data(3:3:1800000),512)

plot(dataFiltered,'r');



