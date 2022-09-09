#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "GBK_LibDrive.h"	
extern uint8_t SendData[24];
extern char USART_REC_Finish_FLAG;
u8 x=0;
u8 lcd_id[12];			
int xxx=0,yyy=0,sh_x=0,sh_y=0;
float zy=0,sx=0;
u16 len;
u16 t;
u32 cx=0,cy=0,su=0,jd_s=0,jd_sla=0,zjl=0,zjl_ks=0,jd=0,cy_r=0,zzjl=0;
u16 CX[2000],CY[2000],SU[2000],JD[2000],bian[20],cx_d=0,cy_d=0,x_d=0,y_d=0;
int count_1=0,count_2=0,count_3=0,count_4=0,p=0,fw=0,i=0,j=0,k=0,end=0,end_b,ave_j=0,zong_j=0;
float ave_s=0,zong_s=0;



void big_point(u16 x,u16 y,u16 color)
{
	LCD_Fast_DrawPoint( x, y, color);
	LCD_Fast_DrawPoint( x+1, y, color);
	LCD_Fast_DrawPoint( x-1, y, color);
	LCD_Fast_DrawPoint( x, y+1, color);
	LCD_Fast_DrawPoint( x, y-1, color);
}
	

int main(void)
{	 
	for(i=0;i<=1999;i++)
	{
		CX[i]=900;
		CY[i]=900;
		SU[i]=900;
	}
	for(i=0;i<=19;i++)
	{
	  bian[i]=900;
	}
	delay_init();	    
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
	uart_init(115200);	
	LED_Init();			
	LCD_Init(); 
	POINT_COLOR=RED;


	LCD_ShowString(0,60,60,16,16,BLACK,"speed");
	LCD_ShowString(0,90,60,16,16,BLACK,"angle");
	LCD_ShowString(0,120,60,16,16,BLACK,"distance");
	LCD_ShowString(190,60,60,16,16,BLACK,"cm/s");
	LCD_ShowString(190,90,60,16,16,BLACK,"du");
	LCD_ShowString(190,120,60,16,16,BLACK,"cm");

	while(1)
	{
		if(USART_RX_STA&0x8000)
		{					   
			len=USART_RX_STA&0x3fff;

			if((SendData[0]-'0')==3)
			{
				LCD_Clear(WHITE);
				for(i=0;i<=1999;i++)
				{
					if(CX[i]==900)
					{
					  break;
					}
				}
				end=i;
				for(i=0;i<=end;i++)
				{
					y_d=(int)((440-CY[i])*0.355);
					x_d=(int)(CX[i]*0.355);
					cx_d=115+(x_d-115)*(1-y_d/320)+5;
					cy_d=y_d+5;

					cx_d=115+(int)((cx_d-115)*(1-cy_d*0.0018));

					big_point(cx_d,cy_d,BLUE);
					//delay_ms(2);
				}

				for(i=0;i<20;i++)
				{
					if(bian[i]==900)
					{
					  break;
					}
				}
				end_b=i;


				//判断速度突变点
				//舍去角度突变点的部分（因为转弯的时候在摄像头里必有速度突变）
				fw=(int)(3/140)*end;
				for(i=0;i<=end;i++)
				{
					for(j=0;j<=end_b;j++)
					{
						if((i<=(bian[j]-fw))||(i>=(bian[j]+fw)))
						{
						  count_3++;
						}
					}
					if(count_3==(end_b+1))
					{
						if(((SU[i]-SU[i-1])>=2)||((SU[i+1]-SU[i])>=2))
						{
						  count_4++;
						  bian[end_b+count_4]=i;
						}
					}
				}

				for(i=0;i<=end_b;i++)
				{
					y_d=(int)((440-CY[bian[i]])*0.355);
					x_d=(int)(CX[bian[i]]*0.355);
					cx_d=115+(x_d-115)*(1-y_d/320)+10;
					cy_d=y_d+5;
					cx_d=115+(int)((cx_d-115)*(1-cy_d*0.0018));
					//LCD_Draw_Circle(cx_d,cy_d,3,RED);
					LCD_Fill(cx_d-3,cy_d-3,cx_d+3,cy_d+3,RED);
				}
				for(i=end_b+1;i<=end_b+count_4;i++)
				{
					y_d=(int)((440-CY[bian[i]])*0.355);
					x_d=(int)(CX[bian[i]]*0.355);
					cx_d=115+(x_d-115)*(1-y_d/320)+10;
					cy_d=y_d+5;
					cx_d=115+(int)((cx_d-115)*(1-cy_d*0.0018));
					//LCD_Draw_Circle(cx_d,cy_d,3,GREEN);
					LCD_Fill(cx_d-3,cy_d-3,cx_d+3,cy_d+3,GREEN);
				}

				for(i=0;i<=end_b+count_4-1;i++)
				{
					k=i;
					for(j=i+1;j<=end_b+count_4;j++)
					{
						if(bian[k]<bian[j])
						{
						  k=j; 
						}
						if(k!=i)
						{
							p=bian[k];
							bian[k]=bian[i];
							bian[i]=p;

						}
					}
				}

				for(i=0;i<=end_b+count_4-1;i++)
				{
					if(i==0)
					{
						//SU[bian[i]]
						for(j=0;j>=bian[i];j++)
						{
							zong_s+=SU[j]/200.0;
							zong_j+=JD[j];
						}
						ave_s=zong_s/bian[i];
						ave_j=zong_j/bian[i];
						zong_s=0;
						zong_j=0;
					}
					else
					{
						for(j=bian[i-1];j<=bian[i];j++)
						{
							zong_s+=SU[j]/200.0;
							zong_j+=JD[j];
						}
						ave_s=zong_s/(bian[i]-bian[i-1]);
						ave_j=zong_j/(bian[i]-bian[i-1]);
						zong_s=0;
						zong_j=0;
					}

					LCD_ShowNum(0,180+i*13,(int)(ave_s*200)/200,3,12,BLACK);
					LCD_ShowNum(32,180+i*13,(int)(ave_s*200)%200,3,12,BLACK);
					LCD_ShowNum(130,180+i*13,ave_j,4,12,BLACK);
					LCD_ShowNum(190,180+i*13,bian[i],4,12,BLACK);
				}
				count_3=0;
				count_4=0;
				while(1)
				{
				  LCD_ShowString(100,300,60,16,16,BLACK,"FINISH");

				}
			}

			//接收数据帧
			else if((SendData[0]-'0')==1)
			{
			  	cx=(SendData[0]-'0')*1000+(SendData[1]-'0')*100+(SendData[2]-'0')*10+(SendData[3]-'0')*1-1000;
			  	cy=(SendData[4]-'0')*1000+(SendData[5]-'0')*100+(SendData[6]-'0')*10+(SendData[7]-'0')*1-2000;
			  	su=(SendData[8]-'0')*10000+(SendData[9]-'0')*1000+(SendData[10]-'0')*100+(SendData[11]-'0')*10+(SendData[12]-'0')*1-30000;
				  jd_s=(SendData[13]-'0')*1000+(SendData[14]-'0')*100+(SendData[15]-'0')*10+(SendData[16]-'0')*1;
				if((jd_s>=4000)&&(jd_s<5000))
				  jd=jd_s-4000;
				else if((jd_s>=5000)&&(jd_s<6000))
				  jd=jd_s-5000;
				else if((jd_s>=6000)&&(jd_s<7000))
				  jd=jd_s-6000;
				else if((jd_s>=7000)&&(jd_s<8000))
				  jd=jd_s-7000;
				zzjl=(SendData[17]-'0')*100000+(SendData[18]-'0')*10000+(SendData[19]-'0')*1000+(SendData[20]-'0')*100+(SendData[21]-'0')*10+(SendData[22]-'0')*1;
				//cy_r=(SendData[23]-'0')*1000+(SendData[24]-'0')*100+(SendData[25]-'0')*10+(SendData[26]-'0')*1-8000;
				CX[count_1]=cx;
				CY[count_1]=cy;
				SU[count_1]=su;
				JD[count_1]=jd;
				if(count_1==2)
			  	zjl_ks=zjl;
				if(((jd_sla/1000)!=(jd_s/1000))&&(jd_sla!=0))
				{
					bian[count_2]=count_1;
					count_2++;
				}

				count_1++;
				if(count_1>=1000) 
					count_1=1000;

				LCD_ShowNum(70,60,(su/200),2,16,BLACK);
				LCD_ShowString(102,60,60,16,16,BLACK,".");
				LCD_ShowNum(116,60,((su/2)%100),2,16,BLACK);
				LCD_ShowNum(70,90,jd,6,16,BLACK);
				LCD_ShowNum(70,120,zzjl-900000,8,16,BLACK);

				jd_sla=jd_s;
			}

			USART_RX_STA=0;
		}
	}
}


