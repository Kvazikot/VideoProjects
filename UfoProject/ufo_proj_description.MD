  + - - - + - + - -
  + - + - + copyright by Vladimir Baranov (Kvazikot)  <br>
  + - + - + email: vsbaranov83@gmail.com  <br>
  + - + - + github: http://github.com/Kvazikot/VideoProjects  <br>
```
                            )            (
                           /(   (\___/)  )\
                          ( #)  \ ('')| ( #
                           ||___c\  > '__||
                           ||**** ),_/ **'|
                     .__   |'* ___| |___*'|
                      \_\  |' (    ~   ,)'|
                       ((  |' /(.  '  .)\ |
                        \\_|_/ <_ _____> \______________
                         /   '-, \   / ,-'      ______  \
                b'ger   /      (//   \\)     __/     /   \
                                            './_____/
```     
# Сценарий клипа к переводу подкаста Рогана и Каку про НЛО.
Первые 10 минут аудио я перевел. 

##  Основные элементы клипа:
1. Сцена на фоне апокалиптического индустриального пейзажа. Подготовить 4 фона.

2. 2 Старых телевизора на переднем плане.
НА одном ТВ идет видео каку и рогана (#1) первые 14 минут
На втором разные клипы
3. Человек в кожанной куртке около бочки с огнем. Смотрит Тв.
4. В начале клипа он переключает каналы. 
5 Подготовить в Юнити отдельную сцену прилета летающей тарелки.
* Человек в кожанке отбивается от пришельцев используя лопату.
* Снять человека в кожанке можно на втором этаже.
* Синхронизировать ее с вилдео с веб камеры чтобы было видно где ЛА.
* Можно вставить анимацию оконтуривания как ударом энергетического луча.
6. Летательный аппарат летает по формуле Тейлора для углов и y позиции.
7. Сцена может меняться на футуристическую
8. Сделать аналоговые помехи на телевизоре.

##  Сценарий "Дикарь будущего против пришельцев"
1. Начинается с показа заставки MTV
2. Потом кожанка переключает каналы переключателем.
3. Смотрит несколько клипов Джексона.
4. Нарывается на подкаст Каку и рогана.
5. Тут прилетает тарелка.
6. Он начинает махать граблями, лопатами.
7. Отгоняет тарелку.

## Сценарий "Дикарь из будущего ходит на заднем плане подкаста"

## Сценарий "Дикарь из будущего ходит в телевизоре внутри сцены подкаста"

## Сценарий "Роган и каку находятся в телевизоре апокалиптической сцены"

## Видео источники:
* UFO: Pentagon releases three leaked videos - is the truth finally out there?
* Клипы Джексона с Earph song 
* remember time 
* видео Гитлера у трибуны
* Шкала Кардашева
* телепатия
* язык животных, Канзи


### Начальный скрипт для движения НЛО (C#). Сцену можно взять дефолтувую для 2021 Unity HDRP с золотым шариком 
```
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UfoMotion : MonoBehaviour
{
	public float t=0;
	public float f = 0.1f;
	public Vector3 A;
	public Vector3 pos0;
	public float y = 0;
	const float t_ost = 0.0001f;
    // Start is called before the first frame update
    void Start()
    {
		t = 0;
		f = 0.1f;
		A = new Vector3(0, 0.01f ,0);
		pos0 = new Vector3(0, 0f ,0);
        
    }
	
	float Getvz(float t)
	{
		float vz = A.y * Mathf.Sin(2.0f * Mathf.PI * f * t);		
		return vz;
	}
	
	float Getaz(float t)
	{
		const float az = 1;
		return az;
	}
	
	
    float CalcTeilor(float dt, float r, float v, float a, float t_ost)
	{
		float val;
        val = r + v * dt + (a * dt * dt) / 2f + t_ost;
		return val;
	}
			
	float CalcZ(float dt, float t)
	{
		return CalcTeilor(dt, pos0.z, Getvz(t), Getaz(t), t_ost);
	}
		
    // Update is called once per frame
    void Update()
    {
		t+=Time.deltaTime;
		y = CalcZ(Time.deltaTime, t);
		transform.position = transform.position + new Vector3(0,y,0);
		
    }
}

```
