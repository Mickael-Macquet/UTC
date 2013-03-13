import jade.core.Profile;
import jade.core.ProfileImpl;
import jade.core.Runtime;
import jade.wrapper.AgentContainer;
import jade.wrapper.AgentController;

public class Initial {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		Runtime rt = Runtime.instance();
		System.out.println("COUCOU");
		Profile p = null;
		try {
		  p = new ProfileImpl("properties");
		  AgentContainer mc = rt.createMainContainer(p);
		  
		  AgentController amult = mc.createNewAgent(
		    "MultService",
		    "MULT", null);
		  amult.start();
		  AgentController afact = mc.createNewAgent(
				    "facto",
				    "FACT", null);
		  afact.start();
			
		}
		catch(Exception ex) {System.out.println("Exception creation d'agents");}
	
	}
}
