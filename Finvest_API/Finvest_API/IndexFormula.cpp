/*
    IndexFormula.cpp
    * A set of fomulars
    * author: Finvest
*/
#include <stdio.h>
#include <string>
#include <math.h>
#include "DBAccess.h"
#include "IndexFormula.h"
/*
    Index List
        bool RSI(); // RSI
        bool MACD(); // MACD =======����
        bool Stochastics(); // ����ĳ��ƽ ========�ϼ�
        bool RateOfChange(); // ��ȭ�� =========�ϼ�, ��� ����
        bool PivotPoint(); // Pivot Point =========����, ��� ����
        bool SonarMomentum(); // Sonar Momentum =========����
        bool TRIX(); // TRIX ===========����
        bool CCI(); // CCI ==========�ϼ�, �������
        bool VR(); // VR 
        bool RCI(); // RCI
        bool Disparity(); // �̰ݵ�
        bool NCO(); //NCO, �������
        bool PriceOS(); //Price Oscillator, �������
        bool ADLine(); //ADLine 
*/

void IndexFormula::init()
{
    printf("Enter the stock name: ");
    std::string name;
    std::cin >> name;
    if(db_acc.SetStock(name))
    {
        printf("Load data complete \n");
    }
}

 void GetDisparityValue()
 {
	 //20�� �̰ݵ�
	// int DV20= db_acc.GetClose();
 }

int IndexFormula::GetMACDValue()
{

	int short_sum, long_sum, cnt;
	int short_average, long_average;
	int macd_line;

	int* short_average_arr = db_acc.GetClose(12); //�ܱ��̵���ռ��� ���� 12�ϵ����� ����
	int* long_average_arr = db_acc.GetClose(26); //����̵���ռ��� ���� 26�ϵ����� ����
	
	for(cnt = 0; cnt < 12; cnt++)
	{
		short_sum += short_average_arr[cnt];
	}

	for(cnt = 0; cnt < 26; cnt++)
	{
		long_sum += long_average_arr[cnt];
	}

	short_average = short_sum / 12; //�ܱ��̵���ռ�
	long_average = long_sum / 26; //����̵���ռ�

	macd_line = short_average-long_average; //macd�

	return macd_line;
}

int IndexFormula::GetRSIValue()
{
    //RSI = {(14�ϰ� ����� �հ�) / (14�ϰ� ����� �հ� + 14�ϰ� �϶��� �հ�) } * 10
    return 0;
}

/* s :: hyeyeong.ahn - 2012. 04. 30 */
int IndexFormula::GetStochasticValue()
{
    /*
        Stochastic
        (���� - 5�ϵ����� ������) / (5�ϵ����� �ְ� - 5�ϵ����� ������)
    */
    int st_k;
    int* low_arr = db_acc.GetLow(5);
    int* high_arr = db_acc.GetHigh(5);
    int min_low, max_high;
    int close = db_acc.GetTodayClose();

    min_low = low_arr[0];
    max_high = high_arr[0];

    for(int cnt = 1; cnt < 5; cnt++)
    {
        if(low_arr[cnt] < min_low)
        {
            min_low = low_arr[cnt];
        }

        if(high_arr[cnt] > max_high)
        {
            max_high = high_arr[cnt];
        }
    }

    st_k = (close - min_low) / (max_high - min_low);
    return st_k;
}

/* s --hyeyeng.ahn - 2012. 04. 29 */
int IndexFormula::GetRatioValue() 
{
    /*
        ��ȭ��
        ���̳ʽ� �������� 0���� ����Ͽ� �÷����� ���� �ż�
        �÷��� �������� 0���� ����Ͽ� ���̳ʽ��� ���� �ŵ�
        ��ȭ�� = {(�������� - 12���� ����) / 12���� ���� } * 100
    */

    float ratio;
    int t_close = db_acc.GetTodayClose();
    int p_close = db_acc.GetPrevClose(12);

    printf("today close: %d \t prev close: %d \n", t_close, p_close);

    ratio = ((t_close - p_close) / (float)p_close) * 100;
    printf("ratio: %f \n", ratio);
    return (int)ratio;
}
/* e -- hyeyeng.ahn - 2012. 04. 29 */

/* s -- hyojin.kim * - 2012. 05. 01 */
int IndexFormula::GetPivotValue()
{
    //pivot point = (������ �� + ������ ���� + ������ ����) / 3
    int y_high = db_acc.GetPrevHigh(1);
    int y_low = db_acc.GetPrevLow(1);
    int y_close = db_acc.GetPrevClose(1);
    printf("%d \t %d \t %d \t \n", y_high, y_low, y_close);

    int ppoint = (y_high + y_low + y_close) / 3;

    /*
        res: ���׼�
        sup: ������
    */
   // int res1 = (2 * ppoint) - y_low;
   // int sup1 = (2 * ppoint) - y_high;
  //  int res2 = (ppoint - sup1) + res1;
    //int sup2 = ppoint - (res1 - sup1);

    return ppoint;
}

int IndexFormula::GetTodayCloseValue()
{
    return db_acc.GetTodayClose();
}

/*int IndexFormula::GetPivotResValue1()
{
    int res1 = 0; //1�� ���׼�
    int y_low = db_acc.GetPrevLow(1);

    res1 = (2 * GetPivotValue()) - y_low;

    return res1;
}

int IndexFormula::GetPivotResValue2()
{
    int sup1 = 0; //1�� ������
    int y_high = db_acc.GetPrevHigh(1);

    sup1 = (2 * GetPivotValue()) - y_high;

    return sup1;
}*/

///* e -- hyojin.kim  http://blog.daum.net/tax8282kds/15848281 */

/* s -- hyeyeng.ahn - 2012. 04. 29 */
int IndexFormula::GetCCIValue()
{
    /* 
        CCI = (M - m) / (d * 0.015)
        M(��հ���) = (������ �� + ������ ���� + ������ ����) / 3
        m = M�� n�� �ܼ��̵����
          = M + M + .... M / 20
    */
    int* h_arr = db_acc.GetHigh(20);
    int* l_arr = db_acc.GetLow(20);
    int* c_arr = db_acc.GetClose(20);

    int M[20];
    int m_sum = 0;
    int d_sum = 0;
    int n_cci = 0;
    int m, d, cnt; // cnt: for loop index

    for(cnt = 0; cnt < 20; cnt++)
    {
        M[cnt] = (h_arr[cnt] + l_arr[cnt] + c_arr[cnt]) / 3;
        m_sum += M[cnt];
    }
    m = m_sum / 20;

    int abs_d[20];
    for(cnt = 0; cnt < 20; cnt++)
    {
        abs_d[cnt] = abs(M[cnt] - m);
        d_sum += abs_d[cnt];
    }
    d = d_sum / 20;

    n_cci = (M[0] - m) / (d * 0.015);
    
    return n_cci;
}
/* e -- hyeyeng.ahn - 2012. 04. 29 */

/* s -- hyeyeng.ahn - 2012. 04. 30 */
void IndexFormula::GetSonarValue()
{
    /*
        sonar momentum = (���������̵���� - n���� �����̵����) / n���� �����̵����
        sonar momentum signal = sonar momentum�� m�� �����̵����
        m = 5, n = 10 ��õ
     */
     int today_ema = GetEMA(1, db_acc.GetClose(1));
     int nday_ema = GetEMA(10, db_acc.GetClose(10));
     //int sonar_m_signal[5];
     /*for(int cnt = 0; cnt < 5; cnt++)
     {
        sonar_m_signal[cnt] = 
     }
     sonar momentum�� n�ϵ��� ���ؾ� ��? -> ���� ���� �̵� ��� 5��.. ��¥�� 5�� �ٲ�����;;
     */
}


/*void IndexFormula::GetTrixValue()
{
	//EMA1 = ������ N�� ���� �̵����(Exponential Moving Average)  
	//EMA2 = EMA1�� N�� ���� �̵����  
	//EMA3 = EMA2�� N�� ���� �̵���� 
	//TRIX = (������ EMA3�� - ������ EMA3��) / ������ EMA3�� TRIX Siganl 
       // = TRIX�� M�� �����̵���� 
	//TRIX signal = TRIX�� K���� �����̵����

	int EMA1 = IndexFormula::GetEMA(15, );
	int EMA2 = 
}*/

/* s -- hyojin.kim - 2012. 05. 01 */
int IndexFormula::GetNCOValue()
{
   /*
        momentum = (�ֱ����� - 12���� ����)
   */
    int momentum = 0;
    momentum = (db_acc.GetTodayClose() - db_acc.GetPrevClose(12));
    return (int) momentum;
}

int IndexFormula::GetPriceOSValue(){

    /*
        priceoscillator = ((ma(����, �ܱ�) - ma(����, ���)) / ma (����, �ܱ�)) * 100
        ma(�ܼ��̵����) = (n�ϵ����� ������ ��)/ n��
    */
    float pos;//price oscillator
    int mas[6]; //�ܱ��̵����
    int mal[75]; //����̵����
    int ms, ml, cnt; //cnt : for loop index
    int ps_sum = 0; //�ܱ� ������ ��
    int pl_sum = 0; //��� ������ ��
    int* s_close = db_acc.GetClose(6); //6�ϵ����� ����
    int* l_close = db_acc.GetClose(75); //75�ϵ����� ����

    for(cnt = 0; cnt < 6; cnt++)
    {
        ps_sum += s_close[cnt];
    }
    ms = ps_sum / 6;

    for(cnt = 0; cnt < 75; cnt++)
    {
        pl_sum += l_close[cnt];
    }
    ml = pl_sum/ 75;

    pos = ((ms - ml) / (float)ms) * 100;

    return (int)pos;
}

int IndexFormula::GetTADLineValue(){

	/*
	AccDist = ((((���� - ����) - (�� - ����)) / (�� - ����)) * �ŷ���) + ����AccDist
	*/

    int taccdist; //���� accdist
    int prevaccdist;
    int tclose, tlow, thigh = 0; //������ ����, ����, ��
    int volume; //�ŷ���

    tclose = db_acc.GetTodayClose();
    tlow = db_acc.GetTodayLow();
    thigh = db_acc.GetTodayHigh();
    volume = db_acc.GetTodayVolume();

    taccdist = ((((tclose - tlow) - (thigh - tlow)) / (thigh - tlow)) * volume) + GetPrevADLineValue();

    return taccdist;
}

int IndexFormula::GetPrevADLineValue(){

	/*
	AccDist = ((((���� - ����) - (�� - ����)) / (�� - ����)) * �ŷ���) + ����AccDist
	*/
   
    int prevaccdist = 0; //���� accdist
    int prevclose, prevlow, prevhigh = 0; //������ ����, ����, ��
    int prevvolume; //�ŷ���

    prevclose = db_acc.GetPrevClose(1);
    prevlow = db_acc.GetPrevLow(1);
    prevhigh = db_acc.GetPrevHigh(1);
    prevvolume = db_acc.GetPrevVolume(1);

    //prevaccdist = ((((prevclose - prevlow) - (prevhigh - prevlow)) / (prevhigh - prevlow)) * prevvolume) + GetPrevADLineValue();
    //�ٵ� ���⼭ GetPrevADLineValue�� �� ����ؼ� �� �� ���� accdist���� �ҷ��־����. �̰� �������� �˾ƾ���.

    return prevaccdist;
}
/* e -- hyojin.kim - 2012. 05. 01 */



/*
    private functions
*/
int IndexFormula::GetSignalValue()
{
	int* macd_arr = new int[9]; //9�ϵ����� macd�����̵���� - 9�ϵ����� macd���� �迭�� �ִ°��� ������~~
	int macd_sum = 0;
	int signal_line;

	for(int i = 0 ;  i < 9; i++)
	{
		macd_sum += macd_arr[i];
	}

	signal_line = macd_sum/9; //�ñ׳� �

	return signal_line;
}

int IndexFormula::GetEMA(int day, int* data)
{
    /*
        �����̵���� exponential moving average
        1��: 11,200
        2��: 10,800 * 0.33 + 11,200 * 0.67 = 11,068
        3��: (���� * k) + (���� �����̵����(11,068) * (1-k)) = 3��° �����̵����
    */
    int cnt, sum = 0;

    // ��Ȱ���
    float k = 2 / (float)(day + 1);
    float ma;

    ma = data[0];
    if(day > 1)
    {
        for(cnt = 1; cnt < day; cnt++)
        {
            float ck = data[cnt] * k;
            float mk = ma * (1-k);
            ma = ck + mk;
        }
    }
    else if(day == 1)
    {
        float ck = data[0] * k;
        float mk = ma * (1-k);
        ma = ck + mk;
    }

    return (int) ma;
}
/* e -- hyeyeng.ahn - 2012. 04. 30 */