if(dup2_error < 0) { //error check
    perror(NULL);
    exit(-9);
}