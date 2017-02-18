int16_t arduino_get_sock_num(){
  for(int16_t i=0; i<MAX_SOCK_NUM; i++){
    uint8_t st = socketStatus(i);
    if(st==SnSR::CLOSED){
      return i;
    }
  }
  return -1;
}
int accept(SOCKET s){
  do{
    uint8_t st = socketStatus(s);
    if((st&SnSR::ESTABLISHED)==SnSR::ESTABLISHED){
      return 1;
    }
  }while(1);
}
