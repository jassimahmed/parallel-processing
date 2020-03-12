#include "A4_sort_helpers.h"

sem_t *sem_array[27];

// Reads an entire file, line by line.
void read_all( char *filename ){

    FILE *fp = fopen( filename, "r" );
    int curr_line = 0;

    while( curr_line < MAX_NUMBER_LINES &&
           fgets( text_array[curr_line], MAX_LINE_LENGTH, fp ) )
    {
        curr_line++;
    }

    text_array[curr_line][0] = '\0';
    fclose(fp);
}

// Reads only the lines of a file staring with first_letter.
void read_by_letter( char *filename, char first_letter ){

    FILE *fp = fopen( filename, "r" );
    int curr_line = 0;
    text_array[curr_line][0] = '\0';

    while( fgets( text_array[curr_line], MAX_LINE_LENGTH, fp ) ){
        if( text_array[curr_line][0] == first_letter ){
            curr_line++;
        }

        if( curr_line == MAX_NUMBER_LINES ){
            sprintf( buf, "ERROR: Attempted to read too many lines from file.\n" );
            write( 1, buf, strlen(buf) );
            break;
        }
    }

    text_array[curr_line][0] = '\0';
    fclose(fp);
}

// Sorts word using insertion sort
void sort_words( ){
  char tmp[MAX_LINE_LENGTH];
  int min_word_pos;
  char curr[MAX_LINE_LENGTH];

  for (int i = 0; i < MAX_NUMBER_LINES && text_array[i][0] != '\0'; i++) {
      strcpy(curr, text_array[i]);
      min_word_pos = i;

      for (int j = i; j < MAX_NUMBER_LINES && text_array[j][0] != '\0'; j++) {
          if (strcmp(curr, text_array[j]) > 0) {  // this means text_array at this position if lesser than curr
            strcpy(curr, text_array[j]);
            min_word_pos = j;
          }
      }

      if (text_array[i][0] != '\0') {
        strcpy(tmp, text_array[i]);
        strcpy(text_array[i], curr);
        strcpy(text_array[min_word_pos], tmp);
      }

      // now curr is lowest alphabetically

  }

  text_array[MAX_NUMBER_LINES][0] = '\0';
}

// Setups variables and files to coordinate processes. Occurs before any fork() has been called.
// It's run in the original parent process.
int initialize( ){
    char letters[27][15] = { "260707197_a", "260707197_b", "260707197_c", "260707197_d", "260707197_e", "260707197_f", "260707197_g", "260707197_h", "260707197_i", "260707197_j", "260707197_k", "260707197_l", "260707197_m", "260707197_n", "260707197_o", "260707197_p", "260707197_q", "260707197_r", "260707197_s", "260707197_t", "260707197_u", "260707197_v", "260707197_w", "260707197_x", "260707197_y", "260707197_z", "last_sem" };

    for (int i = 0 ; i < 27; i ++ ) {
      if ( i == 0){
        sem_unlink( letters[i] ); // to reset semaphore value bc it's from kernel
        sem_array[i] = sem_open( letters[i], O_CREAT, 0666, 1 );
      } else {
        sem_unlink( letters[i] );
        sem_array[i] = sem_open( letters[i], O_CREAT, 0666, 0 );
      }
    }

    return 0;
}

int process_by_letter( char* input_filename, char first_letter ){
    int pos = (int) first_letter - 'a';
    sem_wait(sem_array[pos]);
    read_by_letter(input_filename, first_letter);
    sort_words();
    FILE * fp = fopen("tmp.txt", "a");

    for (int i = 0 ; i < MAX_NUMBER_LINES && text_array[i] != '\0'; i++ ){
      fputs(text_array[i], fp);
    }
    fclose(fp);


    sem_post(sem_array[pos+1]);
    return 0;
}

int finalize( ){
    sem_wait(sem_array[26]);

    char c;
    FILE *fp = fopen("tmp.txt", "r");
    if (fp) {
      while ((c = getc(fp)) != EOF ) {
        putchar(c);
      }
    }
    fclose(fp);

    sprintf( buf, "Sorting complete!\n" );
    write( 1, buf, strlen(buf) );

    system("rm tmp.txt");

    return 0;
}
