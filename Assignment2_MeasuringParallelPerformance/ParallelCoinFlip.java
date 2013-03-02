import java.util.Random;

/**
 * hw4c
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
 * Class used to demonstrate parallelize coin flips.
 */
public class ParallelCoinFlip implements Runnable {

    //private static Random rand = new Random();
    private static int numHeads = 0;
    
    /** The number of flips this ParallelCoinFlipper is responsible for. */
    private int numFlips;
    
    /**
     * Constructor.
     * @param flips
     *              The number of flips this PCF will be responsible for.
     */
    public ParallelCoinFlip(int flips) {
        this.numFlips = flips;
    }
    
    /**
     * Parallel entry point.
     */
    @Override
    public void run() {
        int localNumHeads = 0;
        Random inRand = new Random();
        
        for (int i = 0; i < this.numFlips; i++) {
            if (inRand.nextBoolean()) {
                localNumHeads++;
            }
        }
        synchronized (ParallelCoinFlip.class) {
            ParallelCoinFlip.numHeads += localNumHeads;
        };
    }

    /**
     * Program starts here.
     * @param args
     *              Command line args.
     */
    public static void main(String[] args) {
        if (args.length != 2) {
            System.err.println("Usage:\nParallelCoinFlip #threads #coinFlips");
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
        
        int flipsPerThread = flips / numThreads;
        //Log our start time
        long startTime = System.currentTimeMillis();
        
        // First loop creates threads with flips / numThreads + 1 iterations
        // just in case numThreads does not perfectly divide into flips.
        for(int i = 0; i < flips % numThreads; i++) {
            threads[i] = new Thread (new ParallelCoinFlip(flipsPerThread + 1));
            threads[i].start();
        }
        // Make the rest of the threads with flips / numThreads coin flips
        for (int i = flips % numThreads; i < numThreads; i++ ) {
            threads[i] = new Thread (new ParallelCoinFlip(flipsPerThread));
            threads[i].start();
        }
        
        //Join all threads back together
        for (int i = 0; i < numThreads; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                System.out.println("Thread interrupted.  Exception: " + e.toString() +
                        " Message: " + e.getMessage()) ;
                System.exit(-1);
            }
        }
        //Calculate elapsed time.
        long timeElapsed = System.currentTimeMillis() - startTime;
        
        System.out.println(ParallelCoinFlip.numHeads + " heads in " + flips + " coin tosses.");
        System.out.println("Time elapsed: " + timeElapsed + "ms");
    }
}
