import jade.core.Agent;
import jade.core.behaviours.CyclicBehaviour;
import jade.lang.acl.ACLMessage;
import jade.lang.acl.MessageTemplate;

public class MULT extends Agent{
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub

	}
	protected void setup() {
	  	System.out.println("Hello World! Je suis : "+getLocalName());
        addBehaviour(new CyclicBehaviour(this) 
        {
            public void action() 
            {
                ACLMessage msg = receive(MessageTemplate.MatchPerformative(ACLMessage.INFORM));
                if (msg!=null)
                {
                	String str[]=msg.getContent().split("\\*");
                	int valeur1 = Integer.parseInt(str[0]);
                	int valeur2 = Integer.parseInt(str[1]);
                	
                    System.out.println( " - " +
                       myAgent.getLocalName() + " <- " +
                       str[0] +str[1]);
                }
                else
                    block();
            }
        });
	}
	  	
}

