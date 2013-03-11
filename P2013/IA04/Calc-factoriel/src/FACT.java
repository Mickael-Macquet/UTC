import jade.core.Agent;
import jade.core.behaviours.CyclicBehaviour;
import jade.lang.acl.ACLMessage;
import jade.lang.acl.MessageTemplate;

public class FACT extends Agent {

	/**
	 * 
	 */
	public int valeur;
	
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
                    	valeur = Integer.parseInt(msg.getContent());
                        System.out.println( " - " +
                           myAgent.getLocalName() + " <- " +
                           valeur );
                    }
                    else
                        block();
                 }
            });

	} 



}
