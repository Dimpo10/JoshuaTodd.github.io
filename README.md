## My Portfolio

#Meteros
```markdown

## Example of my code in C++

`for (int counter2 = 0; counter2 < MC; counter2++)//first check if meteor is active
	{
		if (ma[counter2] == 1) // is meteor is active
		{
			mr[counter2] -= 0.35; //move meteor 0.35 pixels each frame
			mespritex = screenWidth / 2 - 25 + mr[counter2] * cos(mA[counter2]); //set location of x and y
			mespritey = screenHeight / 2 - 25 + mr[counter2] * sin(mA[counter2]);

			MeteorRender(mespritex, mespritey, meteorSprRect); // render meteors

			if (planethealth == 3) // is the health of the planet is 3, display 3 lives
			{
				RECT lifeSprRect = { 528, 0, 626, 32 };
				LivesRender(20, 20, lifeSprRect);
			}
			if (planethealth == 2)// is the health of the planet is 2, display 2 lives
			{
				RECT lifeSprRect = { 528, 0, 594, 32 };
				LivesRender(20, 20, lifeSprRect);
			}

			if (planethealth == 1)// is the health of the planet is 1, display 1 life
			{
				RECT lifeSprRect = { 528, 0, 562, 32 };
				LivesRender(20, 20, lifeSprRect);
			}

			if (mr[counter2] < 100)//if meteor is at planet radius
			{
				ma[counter2] = 0;//make the meteor inactive	

				planethealth -= 1; // decrement planet health by 1

				if (planethealth < 1) // if plate health is less than 1
				{
					start[z] = 2; // show menu screen
					planethealth = 3; // reset the planets health
				}
			}
			if (start[z] == 2)
			{
				ma[counter2] = 0;
			}

		}
	}`

[Link](https://github.com/Dimpo10/dimpo10.github.io/blob/master/Portfolio/Meteors/Report.pdf) and ![Image](src)
```
