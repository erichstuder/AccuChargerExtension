cd "C:\Users\erich\MyData\Data\Arduino\Projekte\AccuChargerExtension\Software"

h=openserial(27,"9600,n,8,1");

//clear file content
fileHandle=mopen("data.txt", "w")
mclose(fileHandle);

data = "";

cnt = 0;
while(cnt < (10*4500))//sleeps 0.1sec
    buf = readserial(h,4096);
    disp(length(buf));
    
    fileHandle=mopen("data.txt", "a")
    mfprintf(fileHandle,"%s",buf);
    mclose(fileHandle);
    
    //mfprintf(fileHandle,"a \n");
    sleep(100)
    cnt=cnt+1;
end


closeserial(h);
