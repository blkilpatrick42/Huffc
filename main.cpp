//#include "gTree.cpp"
//#include "gPriorityQueue.cpp"
#include "huffCompressor.h"
#include <iostream>
#include <cstring>

int main(int argc,char* argv[]){  
   bool bShowUsage = false;
   if(argc == 3){
      char* option1 = argv[1];
      char* fileName = argv[2];
      huffCompressor fileHuff;
      if(option1[0] == '-'){
         if(option1[1] == 'c' || option1[1] == 'C'){
            int nCompressRate = fileHuff.compress(fileName);
            if(nCompressRate < 0){
               std::cout<<"\nAn error occured. Either the file was too small to be effectively\n";
               std::cout<<"compressed, or a problem was encountered during compression.\n";
            }
            else{
               std::cout<<"\nCompressed by "<<nCompressRate<<"%\n";
            }     
         }
         else if(option1[1] == 'd' || option1[1] == 'D'){
            std::string strFileName(fileName);
            if (strFileName.substr(strFileName.length()-5,5) == ".huff"){
               int nDecompressRate = fileHuff.decompress(fileName);
               if(nDecompressRate < 0){
                  std::cout<<"An error occured. Decompression was either not completed or \nerroneously completed. ";
                  std::cout<<"Make sure this is a valid compressed file.\n";
               }
               else{
                  std::cout<<"Successfully decompressed\n";
               }
            }
            else{
               std::cout<<"An error occured. File does not have .huff extension.\n";
            }
         }
         else{
            bShowUsage = true;
         }
      }
      else{
         bShowUsage = true;    
      }
   }
   else{
      bShowUsage = true;
   }

   if(bShowUsage){
      std::cout<<"|----------------------------------|";
      std::cout<<"\n|Usage:                            |\n";
      std::cout<<"|----------------------------------|";
      std::cout<<"\n|   Compress: huffC -c <fileName>  |";
      std::cout<<"\n| Decompress: huffC -d <fileName>  |";
      std::cout<<"\n|----------------------------------|\n";
   }
}
