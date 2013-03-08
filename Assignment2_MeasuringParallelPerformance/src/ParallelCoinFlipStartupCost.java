import java.util.Random;

/**
 * hw2
 *
 * Kyle Mercer
 * 520.320
 * 03/28/2013
 * HW#2
 * Phone#: 443-386-6211
 * JHED: kmercer5
 * Email: kmercer5@jhu.edu
 */

/**
 * Part 1
 * @author Kyle Mercer
 * Class used to demonstrate parallelize coin flips.
 */
public class ParallelCoinFlipStartupCost implements Runnable {

    /** The number of heads this object/thread flipped */
    private int numHeads;
    
    /** The number of flips this ParallelCoinFlipper is responsible for. */
    private int numFlips;
    
    /**
     * Constructor.
     * @param flips
     *              The number of flips this PCF will be responsible for.
     */
    public ParallelCoinFlipStartupCost(int flips) {
        this.numFlips = flips;
    }
    
    /**
     * Parallel entry point.
     */
    @Override
    public void run() {
        Random inRand = new Random();
        int totalHeads = 0;
        
        for (int i = 0; i < this.numFlips; i++) {
            if (inRand.nextBoolean()) {
                //this.numHeads++;
                totalHeads++;
            }
        }
        this.numHeads = totalHeads;
    }

    /**
     * Program starts here.
     * @param args
     *              Command line args.
     */
    public static void main(String[] args) {
        
        ParallelCoinFlipStartupCost timeKiller =
            new ParallelCoinFlipStartupCost(10 * Integer.parseInt(args[1]));
        
        Thread testThread = new Thread(timeKiller);
        testThread.run();
        System.out.println("Warmed up JVM. Starting startup cost test...");
        
        //Log our start time
        long startTime = System.currentTimeMillis();
        
        if (args.length != 2) {
            System.err.println("Usage:\nParallelCoinFlipStartupCost #threads #coinFlips");
            System.exit(-1);
        }
        
        // Get the number of threads and coin flips we are going to run from the command line
        int numThreads = Integer.parseInt (args[0]);
        int flips = Integer.parseInt(args[1]);
        
        if (numThreads > flips) {
            System.err.println("Number of flips should be greater than or equal to the number of threads");
            System.exit(-1);
        }
    
        // Array to hold references to thread objects
        Thread[] threads = new Thread[numThreads];
        ParallelCoinFlipStartupCost[] flipArr = new ParallelCoinFlipStartupCost[numThreads];
        int flipsPerThread = flips / numThreads;
        
        // We were instructed to ignore rounding errors for flips/thread
        for (int i = 0; i < numThreads; i++) {
            flipArr[i] = new ParallelCoinFlipStartupCost(flipsPerThread);
            threads[i] = new Thread(flipArr[i]);
            threads[i].start();
        }
        
        long timeElapsed = System.currentTimeMillis() - startTime;
        System.out.println("Started all threads took: " + timeElapsed + "ms.");
        
        //Join all threads back together
        for (int i = 0; i < numThreads; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                System.out.println("Thread interrupted.  Exception: "
                        + e.toString() + " Message: " + e.getMessage());
                System.exit(-1);
            }
        }
        //Calculate elapsed time.
        //Calculate total heads from all threads
        int totalHeads = 0;
        for (ParallelCoinFlipStartupCost pcf : flipArr) {
            totalHeads += pcf.numHeads; //can access private member directly since we are in the class itself
        }

        // System.out.println(totalHeads + " heads in " + flips + " coin tosses.");
        // System.out.println("Time elapsed: " + timeElapsed + "ms");
        System.out.println(numThreads + "\t" + timeElapsed);
    }
}
