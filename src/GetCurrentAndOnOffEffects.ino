#include "BluetoothSerial.h"
#include "heltec.h"

#include "Spark.h"
#include "SparkIO.h"

#define PGM_NAME "OnOffEffects"
#define SCR_HEIGHT 10

int scr_line;

SparkIO spark_io(false);
SparkComms spark_comms;

char str[STR_LEN];

unsigned int cmdsub;
SparkMessage msg;
SparkPreset preset;
SparkPreset current;

void printit(char *str) {
  if (scr_line >= 7) {
    Heltec.display->clear();
    Heltec.display->drawString(0, 0, PGM_NAME);
    scr_line = 1;
  }
  Heltec.display->drawString(0,scr_line *8, str);
  Heltec.display->display();
  scr_line++;
}

void dump_preset(SparkPreset preset) {
  int i,j;

  Serial.println(preset.curr_preset); 
  Serial.println(preset.preset_num); 
  Serial.println(preset.UUID); 
  Serial.println(preset.Name); 
  Serial.println(preset.Version); 
  Serial.println(preset.Description); 
  Serial.println(preset.Icon); 
  Serial.println(preset.BPM); 
  
  for (j=0; j<7; j++) {
    Serial.print("    ");
    Serial.print(preset.effects[j].EffectName); Serial.print(" ");
    if (preset.effects[j].OnOff == true) Serial.print(" On "); else Serial.print (" Off ");
    for (i = 0; i < preset.effects[j].NumParameters; i++) {
      Serial.print(preset.effects[j].Parameters[i]); Serial.print(" ");
    }
    Serial.println();
  }

  Serial.println(preset.chksum);
}


void setup() {

//  HWSerial.begin(HW_BAUD, SERIAL_8N1, 33, 32);

  Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Enable*/, true /*Serial Enable*/);

  
  Heltec.display->clear();
  Heltec.display->drawString(0, 0, PGM_NAME);
  Heltec.display->display();
  scr_line = 1;

  spark_io.comms = &spark_comms;

  spark_comms.start_bt();
  spark_comms.connect_to_spark();
  
  spark_comms.start_ser();
  printit("Connected");

  delay(1000);
  spark_io.get_preset_details(0x0100);
}

  
void loop() {

  spark_io.process();

  // Messages from the amp
  
  if (spark_io.get_message(&cmdsub, &msg, &preset)) { //there is something there
    sprintf(str, "< %4.4x", cmdsub);
    printit(str);
    
    if (cmdsub == 0x0301) {
      dump_preset(preset);
      current = preset;

      Serial.print("Turn ");
      Serial.print(current.effects[0].EffectName);
      Serial.println(" off");
      spark_io.turn_effect_onoff(current.effects[0].EffectName, false);

      Serial.print("Turn ");
      Serial.print(current.effects[1].EffectName);
      Serial.println(" on");
      spark_io.turn_effect_onoff(current.effects[1].EffectName, true);
    }
  }

  
}
