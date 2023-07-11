// do not add extra libraries here


/*
    arr       : array to be sorted, in order to get points this array should contain be in sorted state before returning
    ascending : true for ascending, false for descending 
    n         : number of elements in the array
    l         : the number of characters used in counting sort at each time
    
    you can use ceil function from cmath
    
*/
#include<iostream>
#include<cmath>

using namespace std;
int str_to_int(std::string X)
{
    int ind = 0;
    for(int i = X.size()-1; i > -1; i--)
    {
        ind += (X[i]-65)*pow(26,X.size()-1-i);
    }
    return ind;
}
void reverse(std::string A[], int n)
{
    std::string tmp[n];
    for (int i = 0; i < n; i++)
    {
        tmp[i] = A[i];
    }
    for (int i = 0; i < n; i++)
    {
        A[i] = tmp[n-1-i];
    }
}
void counting_sort(std::string A[], std::string B[], int l, int n, int s_i, int &count)
{
    int k = pow(26,l);
    int *C = new int[k+1];
    for (int i = 0; i < k; i++)
    {
        C[i] = 0;
        //count++;
    }
    for (int j = 0; j < n; j++)
    {
        C[str_to_int(A[j].substr(s_i, l))]++;
        count++;
    }
    for (int i = 1; i < k+1; i++)
    {
        C[i] += C[i-1];
        count++;
    }
 /*   for (int j = n-1; j > -1; j--)
    {
        B[C[str_to_int(A[j].substr(s_i, l))]] = A[j];
        C[str_to_int(A[j].substr(s_i, l))]--;
    }*/
    for(int j=n;j>=1;j--){
          B[C[str_to_int(A[j-1].substr(s_i,l))]-1] = A[j-1];
          C[str_to_int(A[j-1].substr(s_i,l))]--;
          count++;
    }
    for (int i = 0; i < n; i++)
    {
        A[i] = B[i];
        count++;
    }
    
}
int radixSort(std::string arr[], bool ascending, int n, int l){
    std::string *tarr = new std::string[n];
    int sz = arr[0].size(), count = 0;

    int j = arr[0].size();
    while(true)
    {
        if(j<=l)
        {
            counting_sort(arr, tarr, j, n, 0, count);
            break;
        }
        counting_sort(arr, tarr, l, n, j-l, count);
        j -= l;
    }
    if(!ascending) reverse(arr, n);
    return count;
}
void printArrayInLine(std::string arr[], int arraySize){
    std::cout << "{ ";
    for(int i = 0; i < arraySize; i++){
        std::cout << """ << arr[i] << """;
        if (i == arraySize - 1){
            continue;
        }else{
            std::cout << ", ";
        }
    }
    std::cout << " }" << std::endl;
}

void test(){
    clock_t begin, end;
    double duration;
    int numberOfIterations;

    int size = 10;      // max 1000000
    int length = 6;     // max 12
    int l = 2;      // number of characters to be used in counting sort (1,2,3,4 or 6)

    std::string arr[200] = { "YOALTQJSVVEX", "KQDKMAPCFPRK", "FXMIQZWPPYCK", "ONDJIJGSBKEN", "KURSLIEQHQYY", "SWPJVRUJEZVO", "IBJJNNYAHRUS", "CYKJPJHJHWSC", "PMMVNJMXKVGY", "IGASYUMCSXLJ", "IVSPTLSJZGGN", "PSKBNTBYZBSZ", "VEBQDPZLKUDD", "FXOEFVTUPGXF", "ZBFACXCABFQF", "URSGLVKSSAZX", "XSTNYRSZUXAW", "WEYZJQGEIZMV", "UWOPZNMWHGJI", "ZDHTCJRANQAX", "GGDQFPOCOCRN", "RDJYLVGMAQHF", "ZZFNRFKZMNQR", "EEVSIMHBSTZD", "OIQQAZVZYBMR", "GYRSLHMSNHKX", "WUYONZTBJLRJ", "LPJJQVBWUURH", "DFBQOOPKINYV", "OUYYFRHQGQCW", "ODVKZMTCRVSF", "JHQTWQOKMOIS", "FSIOKKMAPHKO", "TGQMDKSOTKHR", "CXCPLMHSERIR", "EVRTCEKWKCKP", "PCDIOKARJEIU", "QPOXIXQMSHIU", "NSSXWDOLHRUW", "DWPMAXGSOXRY", "UHLORTKENDEJ", "ISVRKRNPPECR", "DKLTHDSBMDRD", "WCKLFOUPIRGU", "KWKZCMQGWEZE", "HRHVUZBSDLFI", "BCZJVHEGDOHG", "CAMYENENFMIB", "NJWQUDAVFBFB", "ILHMBQUDQIBW", "VIJCWSGJBEBY", "HDVNECQMNXAQ", "PUTIEXEAFOEB", "IMMLQPJYSHNW", "JDKZCLPRHJZM", "IGOPUUSCHENX", "TZXLGMJPRTOT", "GGNOPOAZUQOO", "KGSRMIRFHQSN", "DDEUYVQFBDTS", "RVROLHEVNXPB", "FGIOWCBBFHWD", "COIFRDXKYQYJ", "XFHMCWPJEXXC", "ZAEEHCKKQUPJ", "YPVYFWKFBRTD", "PJMVGLXILBMS", "FYEXTWGTLCRS", "YDZZWVELESIM", "DGUQJJIPJPME", "LVXYZRSXUUYT", "PEEVYNJDVGTE", "RBVAQIFDFEBE", "XWDSQDLFJRAH", "GLKDRDIIGDJZ", "NQCSUGYSCBMS", "GZZPSBWBMIEE", "NMMUSXTHPXCK", "DAEHESZKRYCL", "ZAMOJRSWFGSZ", "GNHVLJHQLLYP", "FZCWAGKBGWPR", "PHQXQIYWYFRL", "QBBCMBTSDXQD", "DCGMBWDSFVPV", "EQTCVNPOOQSC", "UNUXLNCQPIES", "EKLMFCJLSDPO", "SEEGVWIRJFQW", "USPLCTEJFPVL", "RGWKJMABQGJN", "CSGNZWMTQDGT", "YKCEBZPVFLHR", "ZHUQPEFRYMGZ", "IUUZXAUWNYCQ", "ZRLEESXEBSWQ", "YBJYPSZYOTZM", "VTKKTMBSFOYJ", "JWPKQNDQPOOG", "IPEXIFLFAXQT", "JRLQHKBQIRDY", "EIQVWGCFVIED", "OPIQMYKXRXNZ", "JQRRHUROCHLZ", "NPGIYMNOBYEP", "YQOSODRXTIRB", "FIRHSCIHUOSU", "CFIFFOUEFIWV", "NPUHZLIEWBMQ", "FWXZLPTPXDVC", "URIBAGWPVSWX", "EGDCHRSPOPOB", "HKQGPLIJDTMF", "ZKUXDRWJXZLH", "RDWFUMGDWWJO", "KUXNNMUOWONZ", "HJKHLVOCAKHX", "WPAVNMJXGIMV", "WGJTXZUEIFLT", "CBXFNGCMVEHJ", "QSIWCVTZBFSA", "EOFOVSKYUHDJ", "OHVLLEWEWFCB", "CWAFBUGHKLVG", "FFEBPJLFSGQD", "LPJJUMKWKMBN", "GJURWPXCXDDM", "OQRGZJLKYVVU", "JGSTUWGBFCUC", "RTEOYJAMCTSB", "DFNDAIYLQQGL", "MOOSQKUIDAYD", "LBPPUJQZOFDR", "OBEGRLTGZJYQ", "TUAYWYBJZSZW", "DPVUXYNLZRTT", "CPZEYZWSTYSR", "WWAYOZWURROO", "SBBTUWMZLNDK", "MBEHZWAXUBXJ", "CTFVLTLDWOWQ", "LLRWYUINXMWW", "LXWHYTQCNXYA", "SLFOADHLQYJR", "VSESGCRTZNBZ", "ITEXTCXLPECP", "KLCALMRGGXBO", "AUKZJNBRIFPB", "JONYTPQDASDL", "GXUMUXDUTPWC", "CZTKEIMNXBNS", "SDVSWAFCXBRU", "AWQTLMVPLRZR", "ZNGYQUSIZNCX", "QICPLVJMRAFE", "ODTCUUVVICWY", "YOHZCJXUTZJH", "UVTNXARNFMRZ", "GNXOPTPPHYOL", "JNFDORMKOFXL", "HQANCSOJFLZW", "EONMOBXYRFDF", "WPQMWNZDGZSI", "RHTYUTUZJHNY", "LMWCRZHPQZBM", "NAPVAKDTRZSL", "SOMBYBBJOXNF", "YWXOWYCLBUGD", "ELWVMQIEFXID", "YLOMLDUJZRAX", "REKSYSVCGUZS", "KKZRHHWHUMWF", "PQRRJTQAXBVW", "TQYZMAUXKTQT", "CNAWBWCROTKZ", "MAALDXHXPIYC", "IUBSNRLPGNOI", "KQBALLAZNCKR", "ZUQQCQSMLTEA", "NRSTEGDQYGTL", "TTMHXWAYSSOW", "IHIVCOYPHQLM", "YQEWXXJSSVBP", "TBPMVGKGNTBR", "JZJRRUFPMJNL", "JWEBRHTLLKZG", "QJMDCQXORGFJ", "CMAOXOCGKGKE", "NDPYNOHEZTJE", "JISDOZMQLMHK", "CJRNRDTGGIHV", "YOBXJNDTVVYM", "WKEJYNUBWLQP", "OLWWTFTTTXSC", "MWXJTVVQHCZG", "PTJOGBDWMBUH", "GQCZPUEBSDMO", "ZIEGMFOBBZPJ", "CVIQWEZFUBEL", "YKMSOZGNJMVX" };

    std::cout << "Array before sorting:" << std::endl;
    printArrayInLine(arr, size);
    if ((begin = clock() ) ==-1)
        std::cerr << "clock error" << std::endl;

    numberOfIterations = radixSort(arr, true, size, l);

    if ((end = clock() ) ==-1)
        std::cerr << "clock error" << std::endl;

    duration = ((double) end - begin) / CLOCKS_PER_SEC;
    std::cout << "Duration: " << duration << " seconds." << std::endl;

    std::cout << "Number of Iterations: " << numberOfIterations << std::endl;
    std::cout << "Array after sorting:" << std::endl;
    printArrayInLine(arr, size);
}

int main()
{
    //srandom(time(0));
    test();
    return 0;
}
