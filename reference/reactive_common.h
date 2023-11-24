struct averageCounter{
  int *samples;
  int sample_size;
  int counter;

  averageCounter(int size) {
    counter = 0;
    sample_size = size;
    samples = (int*) malloc(sizeof(int) * sample_size);
  }

  bool setSample(int val) {
    if (counter < sample_size) {
      samples[counter++] = val;
      return true;
    }
    else {
      counter = 0;
      return false;
    }
  }

  int computeAverage() {
    int accumulator = 0;
    for (int i = 0; i < sample_size; i++) {
      accumulator += samples[i];
    }
    return (int)(accumulator / sample_size);
  }

};

struct heartbeat_message {
  int client_id;
  int chk;
};
