if(execvp_err < 0) { //error check
    perror(NULL);
    exit(-11);
}