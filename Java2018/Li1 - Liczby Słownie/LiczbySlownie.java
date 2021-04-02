// KODOWANIE: ISO-8859-2

import java.io.*;

class DividedInt {
	
	public boolean negative;
	public int[] parts; 
	
	private static String[] dict = {
		"zero", "jeden", "dwa", "trzy", "cztery",										// 0-4
		"pięć", "sze¶ć", "siedem", "osiem", "dziewięć",									// 5-9
		
		"dziesięć", "jedena¶cie", "dwana¶cie", "trzyna¶cie", "czterna¶cie",				// 10-14
		"piętna¶cie", "szesna¶cie", "siedemna¶cie", "osiemna¶cie", "dziewiętna¶cie",	// 15-19
		
		"dwadzie¶cia", "trzydzie¶ci", "czterdzie¶ci", "pięćdziesi±t",					// 20-23
		"sze¶ćdziesi±t", "siedemdziesi±t", "osiemdziesi±t", "dziewięćdziesi±t",			// 24-27
		
		"sto", "dwie¶cie", "trzysta", "czterysta", "pięćset",							// 28-32
		"sze¶ćset", "siedemset", "osiemset", "dziewięćset",								// 33-36
		
		"tysi±c", "tysi±ce", "tysięcy",													// 37-39
		"milion", "miliony", "milionów",												// 40-42
		"miliard", "miliardy",															// 43-44
		"minus"																			// 45
	};

	public DividedInt(boolean neg, int bi, int mi, int tho, int hun) {
		
		negative = neg;
		parts = new int[4];
		parts[0] = bi;
		parts[1] = mi;
		parts[2] = tho;
		parts[3] = hun;
	}
	
	
	public DividedInt(int numb) {
		
		// ----- ----- BADANIE ZNAKU ----- ----- //
		
		// mamy gwarancje, ze to nie jest IntMIN
		if (numb < 0) {
			negative = true;
			numb = -numb; // zmiana znaku na przeciwny, aby było ok modulo
		}
		
		else negative = false;
			
		
		// ----- ----- INICJACJA TABLICY ----- ----- //
		parts = new int[4];
		for (int i=3; i>=0; i--) {
			parts[i] = numb%1000;
			numb/=1000;
		}
	}
	
	// milion, milionÓW, milionY
	private int typeOfEnding(int numb) {
		if (numb == 1) return 0; // np. milion, tysi±c
		if (numb%100 >= 10 && numb%100 < 20) return 2; // nastki (żeby nie łapało do %2 i %4)
		if (numb%10 >=2 && numb%10 <=4) return 1; // np. milionY, tysi±cE
		return 2; // np. milionÓW, tysięCY		
	}
	
	private String readNumbFromArray(int numb) {
		
		String s = "";
		boolean bSthIsWritten = false; // żeby wiedzieć, czy doklejać spacje
		
		// cyfra setek
		if (numb/100 != 0) {
			bSthIsWritten = true;
			s+=dict[numb/100+27];
		}
		
		// cyfra dziesiatek
		if (numb%100 == 0) 
			return s;
		
		else if (numb%100 <=20) { 
			if (bSthIsWritten) s+=" ";
			bSthIsWritten = true;
			s+=dict[numb%100]; 
			return s;
		}
		
		else {
			if (bSthIsWritten) s+=" ";
			bSthIsWritten = true;
			s+=dict[((numb/10)%10)+18]; // dobieramy się do cyfry dziesi±tek
		}
		
		// cyfra jednosci
		if (numb%10 != 0) {
			if (bSthIsWritten) s+=" ";
			bSthIsWritten = true;
			s+=dict[numb%10];
		}
		
		return s;
	}

	private String getBillion() {
		String s = "";
		s += readNumbFromArray(parts[0]);
		s += " ";
		s += dict[typeOfEnding(parts[0])+43];
		return s;
	}

	private String getMillion() {
		String s = "";
		s += readNumbFromArray(parts[1]);
		s += " ";
		s += dict[typeOfEnding(parts[1])+40];
		return s;
	}
	
	private String getThousand() {
		String s = "";
		s += readNumbFromArray(parts[2]);
		s += " ";
		s += dict[typeOfEnding(parts[2])+37];
		return s;		
	};
	
	private String getHundred() {
		return readNumbFromArray(parts[3]);		
	}
	
	public String getName() {

		String s = "";
		boolean bSthIsWritten = false;
		
		if (negative) {
			bSthIsWritten = true;
			s+=dict[45];
		}
		
		if (parts[0]>0) {
			if (bSthIsWritten) s+=" ";
			bSthIsWritten = true;
			s+=getBillion();
		}
		
		if (parts[1]>0) {
			if (bSthIsWritten) s+=" ";
			bSthIsWritten = true;
			s+=getMillion();
		}
		
		if (parts[2]>0) {
			if (bSthIsWritten) s+=" ";
			bSthIsWritten = true;
			s+=getThousand();
		}
		
		if (parts[3]>0) {
			if (bSthIsWritten) s+=" ";
			bSthIsWritten = true;
			s+=getHundred();
		}
		
		// jesli jeszcze nic nie napisano, to to zero
		if (!bSthIsWritten)
			s+=dict[0];
		
		return s;
	}
}

public class LiczbySlownie {
	

	
	private static String getStrNumb(int x) {
			
		DividedInt dint;
	
		if (x == Integer.MIN_VALUE)
			dint = new DividedInt(true,2,147,483,648);
		else
			dint = new DividedInt(x);
	
		return dint.getName();
	
	}
	
	public static void main(String[] args) {
		
		if (args.length == 0)
			for (int i=0; i<10; i++) {
				int rnd = (int)(Math.random() * 2);
				if (rnd == 0)
					rnd = (int)(Math.random() * (double)(Integer.MIN_VALUE));
				else
					rnd = (int)(Math.random() * (double)(Integer.MIN_VALUE) * -1.0 - 1.0);
				
				System.out.println("["+rnd+"] "+getStrNumb(rnd));
			}
			
		else			
			for (int i=0; i<args.length; i++)
				try {
					int x = Integer.parseInt(args[i]);
					System.out.println(getStrNumb(x));
				}
				catch (NumberFormatException nfe) {
					System.err.println("Nieudana konwersja do Integer: " + args[i]);
				}
	}
}

