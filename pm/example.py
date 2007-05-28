from Slog import spltSlog
import time;
import sys;


while 1:
  stdinstr = sys.stdin.readline();
  if not stdinstr:
    break;
  print stdinstr;
  
#  timeHere = time.localtime()
#  timestr = str(timeHere[0])+'-'+str(timeHere[1])+'-'+str(timeHere[2])+' '+str(timeHere[3])+':'+str(timeHere[4])+':'+str(timeHere[5])+'\n';
  timeHere = time.time()
  timestr = str(int(timeHere));
  print timestr;

  fields = stdinstr.split(", ");
  W = fields[0];
  V = fields[1];
  A = str(float(fields[2])/100);

  print "Watts = ", W, "\tVolts = ",V, "\tAmps = ",A, "\n";

  Data = (('W',W),('A',A),('V', V), ('AID','Printer'),('TimeStamp',timestr))
  spltSlog.spltSlog('PowerMeasure',Data)

