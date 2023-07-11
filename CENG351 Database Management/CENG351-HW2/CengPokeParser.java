import java.io.*;
import java.util.ArrayList;
import java.util.Locale;

public class CengPokeParser {

	public static ArrayList<CengPoke> parsePokeFile(String filename)  {
		ArrayList<CengPoke> pokeList = new ArrayList<CengPoke>();
		File file = new File(filename);
		BufferedReader br
				= null;
		try {
			br = new BufferedReader(new FileReader(file));
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
		// You need to parse the input file in order to use GUI tables.
		// TODO: Parse the input file, and convert them into CengPokes
		String st = null;
		// Condition holds true till
		// there is character in a string
		while (true)
		{
			try {
				if (!((st = br.readLine()) != null)) break;
			} catch (IOException e) {
				e.printStackTrace();
			}
			String[] arrS = st.split("\t");
			pokeList.add(new CengPoke(Integer.parseInt(arrS[1]), arrS[2], arrS[3], arrS[4]));  // add all pokemons to the pokeList
		}
		return pokeList;
	}

	public static void startParsingCommandLine() throws IOException
	{
		// TODO: Start listening and parsing command line -System.in-.
		BufferedReader reader = new BufferedReader(
				new InputStreamReader(System.in));


		while(true)
		{
			String s = reader.readLine();
			String[] arrS = s.split("\t");
			// There are 5 commands:
			// 1) quit : End the app. Print nothing, call nothing.
			switch (arrS[0].toLowerCase()) {
				case "quit":
					return;  // also breaks the loop

				// 2) add : Parse and create the poke, and call CengPokeKeeper.addPoke(newlyCreatedPoke).
				case "add":
					CengPoke poke = new CengPoke(Integer.parseInt(arrS[1]), arrS[2], arrS[3], arrS[4]);
					CengPokeKeeper.addPoke(poke);
					break;

				// 3) search : Parse the pokeKey, and call CengPokeKeeper.searchPoke(parsedKey).
				case "search":
					CengPokeKeeper.searchPoke(Integer.parseInt(arrS[1]));
					break;

				// 4) delete: Parse the pokeKey, and call CengPokeKeeper.removePoke(parsedKey).
				case "delete":
					int n = Integer.parseInt(arrS[1]);
					CengPokeKeeper.deletePoke(n);
					break;

				// 5) print : Print the whole hash table with the corresponding buckets, call CengPokeKeeper.printEverything().
				case "print":
					CengPokeKeeper.printEverything();
					break;
			}
		}
		// Commands (quit, add, search, print) are case-insensitive.
	}
}
