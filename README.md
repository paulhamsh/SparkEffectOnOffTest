# SparkEffectOnOffTest

Some code to get current preset and turn  effects on and off   

```
#include "Spark.h"
#include "SparkIO.h"

SparkIO spark_io(false);
SparkComms spark_comms;

unsigned int cmdsub;
SparkMessage msg;
SparkPreset preset;
SparkPreset current;

void setup() {
  spark_io.comms = &spark_comms;

  spark_comms.start_bt();
  spark_comms.connect_to_spark();
  
  delay(1000);
  spark_io.get_preset_details(0x0100);
}

  
void loop() {
  spark_io.process();

  if (spark_io.get_message(&cmdsub, &msg, &preset)) { //there is something there
    if (cmdsub == 0x0301) {
      current = preset;
      spark_io.turn_effect_onoff(current.effects[0].EffectName, false);
      spark_io.turn_effect_onoff(current.effects[1].EffectName, true);
    }
  }
}

```

