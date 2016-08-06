
% open the port, change COM19 to match your port namclce
%[y1,Fs1]=audioread('37-A.mp3');
%[y2,Fs2]=audioread('39-B.mp3');
mySerial = serial('COM4');
mySerial.baudrate=2400;
mySerial.timeout=1000;
mySerial.Databits=8;
record=zeros(1000,2);
i=1;
fopen(mySerial);
disp('Reading...');
t2=0;
t3=0;
while (1)
%pause(0.02);
tmp=fread(mySerial,1);
t1=cputime;
if (t1-t3>0.05)
    display(tmp);
switch tmp(1,1)
    case 32         %Piano Sound 
    sound(A1,Fs1);
    case 33
    sound(B1,Fs1);
    case 34
    sound(C1,Fs1);
    case 35
    sound(C1,Fs1);
    case 36
    sound(D1,Fs1);
    case 37
    sound(E1,Fs1);
    case 38
    sound(F1,Fs1);
    case 39
    sound(G1,Fs1);
    case 40
    sound(A2,Fs1);
    case 41
    sound(B2,Fs1);
    case 42
    sound(C2,Fs1);
    case 43
    sound(D2,Fs1);
    case 44
    sound(E2,Fs1);
    case 45
    sound(F2,Fs1);
    case 46
    sound(G2,Fs1);
    case 47
    sound(A3,Fs1);
    case 48
    sound(B3,Fs1);
end
t3=cputime;
end
if (t1-t2>0.3)
if ((tmp>=64) && (tmp<96)) %Left Feet Drum
    sound(Bassdrum1/2,Fs2);t2=cputime;record(i,1)=tmp;record(i,2)=cputime;i=i+1;
end
if ((tmp>=96) && (tmp<128)) %Right Feet Drum
    sound(Bassdrum2*2,Fs2);t2=cputime;record(i,1)=tmp;record(i,2)=cputime;i=i+1;
end
end
if ((tmp>=128) && (tmp<150)) %Left Hand Drum
    sound(snaredrum1/2,Fs2);record(i,1)=tmp;record(i,2)=cputime;i=i+1;
else if ((tmp>=150) && (tmp<160))
    sound(snaredrum1*2,Fs2);record(i,1)=tmp;record(i,2)=cputime;i=i+1;
    end
end
if ((tmp>=160) && (tmp<180)) % Right Hand Drum
    sound(snaredrum2/2,Fs2);record(i,1)=tmp;record(i,2)=cputime;i=i+1;
else if ((tmp>=180) && (tmp<192))
    sound(snaredrum2*2,Fs2);record(i,1)=tmp;record(i,2)=cputime;i=i+1;
    end
end
if ((tmp>=192) && (tmp<212)) %Cymbal
    sound(cymbal1/2,Fs2);record(i,1)=tmp;record(i,2)=cputime;i=i+1;
else if ((tmp>=212) && (tmp <224))
    sound(cymbal1*2,Fs2);record(i,1)=tmp;record(i,2)=cputime;i=i+1;   
    end
end
if ((tmp>=224) && (tmp<241)) %Tom
    sound(tom1/2,Fs2);record(i,1)=tmp;record(i,2)=cputime;i=i+1;
else if ((tmp>=241) && (tmp <256))
    sound(tom1*2,Fs2);record(i,1)=tmp;record(i,2)=cputime;i=i+1;   
    end
end
tmp=0
end
% close the port
fclose(mySerial);

disp('Complete');

% close the figure
close all;