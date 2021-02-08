#include<16F877a.h>
#device ADC=8               
// 8 bitlik bir tan�mlama yap�ld� analog giri�ten gelen bilgi 8 bit �zerinden dijital olarak hesaplanacak
#fuses HS, NOWDT, NOLVP, NOBROWNOUT, NOPUT, NOWRT
#use delay (clock = 20000000)

#include <HDM64GS12.c>
#include <graphics.c>

// Step 1 inputs
char giris1[] = {"G_LCD ile Bazi"};
char giris2[] = {"Uygulamalar"};

// Step 2 inputs
char isim[] = {"Ad= "};
char soyisim[] = {"Soyad= "};
char numara[] = {"Ogrenci No="};
char numara_[]  = {"1111111111"};
char program[] = {"Program="};
char program_[] = {"Kontrol ve Otomasyon"};

// Step 3 inputs
char an0_[] = {"Channel_AN0"};
char an5_[] = {"Channel_AN5"};
char an6_[] = {"Channel_AN6"};
char char_analog[] = {"Giris Voltaj"};
char char_digital[] = {"Dijital Karsilik"};

int x1, x2, y1, y2;    // GLCD icin kullan�lacak coordinate saklayan de�i�kenler.
int bar_uzunlugu;
float analog_deger;
float digital_deger;
char convert_analog_deger[15]  ;
char convert_digital_deger[15]  ;

void ayarlar(){
   // Kullanmad���m�z birimler kapat�ld�.
   setup_psp(PSP_DISABLED);
   setup_spi(SPI_SS_DISABLED);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED, 0, 1);
   setup_CCP1(CCP_OFF);
   setup_CCP2(CCP_OFF);
}
void bar_grafigi(int yy2, char* kanal, char* char_analog, char* convert_analog_deger, char* char_digital, char* convert_digital_deger){
      
      x1 = 5 ;
      y1 = 0 ;
      x2 = 5 ;
      y2 = yy2 ;
      glcd_init(ON);
      
      if(y2 == 0) {
         glcd_text57(30, 4, kanal, 1, ON);
         glcd_text57(20, 15, char_analog, 1, ON);
         glcd_text57(30, 25 , convert_analog_deger, 1, ON);
         glcd_text57(20, 35, char_digital, 1, ON);
         glcd_text57(30, 45 , convert_digital_deger, 1, ON);
      }
      else{
         glcd_bar(x1, y1, x2, y2, 10, ON);
         glcd_text57(30, 4, kanal, 1, ON);
         glcd_text57(20, 15, char_analog, 1, ON);
         glcd_text57(30, 25 , convert_analog_deger, 1, ON);
         glcd_text57(20, 35, char_digital, 1, ON);
         glcd_text57(30, 45 , convert_digital_deger, 1, ON);
      }
}

void main(){

   ayarlar();
   
   while(1){
      // Step2 start
      glcd_init(ON);
      glcd_text57(2, 10, giris1, 1, ON);
      glcd_text57(15, 25, giris2, 1, ON);
      delay_ms(600);
      // Step2 end
   
      // Step3 start
      glcd_init(ON);
      // Cerceve �retildi
      glcd_line(2, 1, 126, 1, ON);
      glcd_line(2, 63, 126, 63, ON);
      glcd_line(2, 1, 2, 63, ON);
      glcd_line(126, 1, 126, 63, ON);
      
      // Cerceve icerisine Isim Soyisim Numara Program bilgileri yaz�ld�.
      glcd_text57(4, 5, isim, 1, ON);
      glcd_text57(13, 14, soyisim, 1, ON);
      glcd_text57(22, 23, numara, 1, ON);
      glcd_text57(31, 32, numara_, 1, ON);
      glcd_text57(10, 41, program, 1, ON);
      glcd_text57(4, 49, program_, 1, ON);
      delay_ms(600);
      // Step3 end
      
      
      // Step4 start
      set_tris_a(0x01);   // Analog giri� i�in RA0 kullan�yorum onu input yapt�m.
      set_tris_e(0x03);   // Analog giri� i�in RE0 ve RE1 kullan�yorum input olarak belirlendi.
      
      setup_adc(adc_clock_div_32);    
      // ADC'nin �al��ma frekans�nda 32'de bir par�as� i�in sinyal okuma i�lemi yap�lacak;
      setup_ADC_ports(ALL_ANALOG);    
      // T�m analog giri�ler aktif edildi 16f877a'da toplam 8 tane var ben 3 tanesini kullan�yorum.
      
      
      // For AN0
      set_adc_channel(0);
      delay_ms(100);
      digital_deger = read_adc();                     
      // read_adc() ile dijital deger hesaplandi.
      analog_deger = 0.019607843 * digital_deger ;    
      // 5/255 = 0.196 Digital to Analog // Yukaridaki dijital degere karsilik gelen analog deger bulundu

      sprintf(convert_analog_deger, "%f", analog_deger);    
      // analog deger int tipinde onu char tipine ceviriyoruz.
      sprintf(convert_digital_deger, "%f", digital_deger);  
      // dijital deger int tipinde onu char'a cevirdik.Bunu yapmam�z�n sebebi glcd_text57 parametre olarak pointer char beklemektedir.
   
      bar_uzunlugu = (digital_deger * 64) / 255 ;           
      // y eksen�nde b�r bar olusturacag�z dijital * 64 / 255 gibi basit bir orant� kuruldu.
      bar_grafigi(bar_uzunlugu, an0_, char_analog, convert_analog_deger, char_digital, convert_digital_deger);
      delay_ms(1000);
     
      // For AN5
      set_adc_channel(5);
      delay_ms(100);
      digital_deger = read_adc();              
      analog_deger = 0.019607843 * digital_deger ;   

      sprintf(convert_analog_deger, "%f", analog_deger);    
      sprintf(convert_digital_deger, "%f", digital_deger);  
      
      bar_uzunlugu = (digital_deger * 64) / 255 ;   
      bar_grafigi(bar_uzunlugu, an5_, char_analog, convert_analog_deger, char_digital, convert_digital_deger);
      delay_ms(1000);
       
      // For AN6
      set_adc_channel(6);
      delay_ms(100);
      digital_deger = read_adc();              
      analog_deger = 0.019607843 * digital_deger ;   

      sprintf(convert_analog_deger, "%f", analog_deger);    
      sprintf(convert_digital_deger, "%f", digital_deger); 
      
      bar_uzunlugu = (digital_deger * 64) / 255 ;   
      bar_grafigi(bar_uzunlugu, an6_, char_analog, convert_analog_deger, char_digital, convert_digital_deger);

      delay_ms(1000);
      
   }

}
