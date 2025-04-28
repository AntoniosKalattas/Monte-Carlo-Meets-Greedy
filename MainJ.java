]import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Arrays;
import java.util.Random;

class Args{
    int n, m, c, k;
    int[] set;
    int[][] subsets;
}

public class MainJ{
    //FLAG////////////////////////
    static boolean flag = false;//
    //////////////////////////////
    public static void main(String[] args) {
        if(flag){
            Args myArgs = new Args(); 
            readFile(myArgs, flag);
            System.out.println(myArgs.n + " " + myArgs.m+ " " + myArgs.c+ " " + myArgs.k);
            for(int i =0;i<myArgs.m;i++){
                for(int j=0;j<myArgs.c;j++)
                    System.out.print(myArgs.subsets[i][j] +" ");
                System.out.println();
            }

            alg1(copyArgs(myArgs));
            alg2(copyArgs(myArgs));
            alg3(copyArgs(myArgs));
            alg4(copyArgs(myArgs));
        }else{
            Args myArgs = new Args(); 
            readFile(myArgs, flag);
            runBenchMark(myArgs);
        }     
    }

    public static void runBenchMark(Args args){
        testAlg1(copyArgs(args));
        testAlg2(copyArgs(args));
        testAlg3(copyArgs(args));
        testAlg4(copyArgs(args));
    }
    public static void testAlg1(Args args){
        boolean TIME_LIMIT = false;
        int k =1;
        int[] hittingSet;
        args.k = k;
        while(!TIME_LIMIT){
            double avgT = 0;
            for(int a=0;a<3;a++){
                long startTime = System.nanoTime(); 
                hittingSet = alg1(copyArgs(args));
                long endTime = System.nanoTime();
                if(hittingSet!=null && hittingSet[0]==-1){
                    fwrite("(1) Time Limit for k "+k, "averageTime");  
                    TIME_LIMIT=true;
                    break;
                }
                avgT+=endTime-startTime;
            }
            if(TIME_LIMIT)
                break;
            avgT = avgT/3;
            avgT = (avgT / 1_000_000_000.0);
            fwrite("(1) avg time: "+avgT+ " and k:"+ k, "averageTime");   
            k++;
            args.k = k;            
        }
         
    }
    
    public static void testAlg2(Args args){
        boolean TIME_LIMIT = false;
        int k =1;
        int[] hittingSet;
        args.k = k;
        while(!TIME_LIMIT){
            double avgT = 0;
            for(int a=0;a<3;a++){
                long startTime = System.nanoTime(); 
                hittingSet = alg2(copyArgs(args));
                long endTime = System.nanoTime();
                if(hittingSet!=null && hittingSet[0]==-1){
                    fwrite("(2) Time Limit for k "+k, "averageTime");  
                    TIME_LIMIT=true;
                    break;
                }
                avgT+=endTime-startTime;
            }
            if(TIME_LIMIT)
                break;
            avgT = avgT/3;
            avgT = (avgT / 1_000_000_000.0);
            fwrite("(2) avg time: "+avgT+ " and k:"+ k, "averageTime");   
            k++;
            args.k = k;            
        }
    }
    
    public static void testAlg3(Args args){
        boolean TIME_LIMIT = false;
        int k =1;
        int[] hittingSet;
        args.k = k;
        while(!TIME_LIMIT){
            double avgT = 0;
            for(int a=0;a<3;a++){
                long startTime = System.nanoTime(); 
                hittingSet = alg3(copyArgs(args));
                long endTime = System.nanoTime();
                if(hittingSet!=null && hittingSet[0]==-1){
                    fwrite("(3) Time Limit for k "+k, "averageTime");  
                    TIME_LIMIT=true;
                    break;
                }
                avgT+=endTime-startTime;
            }
            if(TIME_LIMIT)
                break;
            avgT = avgT/3;
            avgT = (avgT / 1_000_000_000.0);
            fwrite("(3) avg time: "+avgT+ " and k:"+ k, "averageTime");   
            k++;
            args.k = k;            
        }
    }
    
    public static void testAlg4(Args args){
        boolean TIME_LIMIT = false;
        int k =1;
        int[] hittingSet;
        args.k = k;
        while(!TIME_LIMIT){
            double avgT = 0;
            for(int a=0;a<3;a++){
                long startTime = System.nanoTime(); 
                hittingSet = alg4(copyArgs(args));
                long endTime = System.nanoTime();
                if(hittingSet!=null && hittingSet[0]==-1){
                    fwrite("(4) Time Limit for k "+k, "averageTime");  
                    TIME_LIMIT=true;
                    break;
                }
                avgT+=endTime-startTime;
            }
            if(TIME_LIMIT)
                break;
            avgT = avgT/3;
            avgT = (avgT / 1_000_000_000.0);
            fwrite("(4) avg time: "+avgT+ " and k:"+ k, "averageTime");   
            k++;
            args.k = k;            
        }   
    }

    public static int readFile(Args args, boolean flag) {
        try (BufferedReader reader = new BufferedReader(new FileReader("test1.dat"))) {
            String[] firstLine = reader.readLine().split("\\s+");
            args.n = Integer.parseInt(firstLine[0]);
            args.m = Integer.parseInt(firstLine[1]);
            args.c = Integer.parseInt(firstLine[2]);
            if(flag) args.k = Integer.parseInt(firstLine[3]);

            args.set = new int[args.n];
            args.subsets = new int[args.m][args.c];

            for (int i = 0; i < args.n; i++) {
                args.set[i] = i + 1; // initialize set with 1 to n
            }

            int i = 0, j = 0;
            String line;
            while ((line = reader.readLine()) != null && i < args.m) {
                String[] nums = line.trim().split("\\s+");
                j = 0;
                for (String numStr : nums) {
                    if (j < args.c) {
                        args.subsets[i][j] = Integer.parseInt(numStr);
                        j++;
                    }
                }
                i++;
            }

            return 0;
        } catch (IOException e) {
            System.err.println("Failed to open file test.dat: " + e.getMessage());
            return -1;
        }
    }


    public static int[] alg1(Args args){
        int hittingSet[] = new int[args.k];

        long startTime = System.currentTimeMillis(); // current time in milliseconds
        long endTime = startTime + 3600 * 1000;      // add 3600 seconds (1 hour), converted to milliseconds

        int map[][] = mapper(args);
        
        int[] valid = new int[args.m];
        for(int i=0;i<args.m;i++)
            valid[i] = i+1;

        int result = algorithm1(args,map,valid,hittingSet,0,endTime);
        if(result ==-1){
            System.out.println("(1) Time limit");
            return new int[]{-1};
        }
        if(result==0)
            System.out.println("(1) failed");
        if(result==1){
            System.out.println("(1) Hitting Set:");
            for(int i=0;i<args.k;i++) System.out.print(hittingSet[i] + " ");
            System.out.println();
            return hittingSet;
        }
        return null;
    }

    public static int algorithm1(Args args, int[][] map, int[] valid, int[] hittingSet, int hittingSetIndex, long endTime){
        int validSubSets = countNonZero(valid);  // Replace length() with countNonZero()
        if(validSubSets<=0 && hittingSetIndex<= args.k) return 1;
        if(hittingSetIndex>=args.k) return 0;
        if(System.currentTimeMillis()>endTime) return -1;
    
        Random rand = new Random();
        int r=rand.nextInt(validSubSets); 
        
        // Find the actual subset index from random index
        int actualIndex = -1;
        int count=0;
        for(int i=0;i<valid.length;i++){
            if(valid[i]!=0){
                if(count == r){
                    actualIndex = valid[i]-1;  // Convert to 0-based index
                    break;
                }
                count++;
            }
        }
        
        for(int i=0;i<args.c && args.subsets[actualIndex][i]!=0;i++){
            hittingSet[hittingSetIndex] = args.subsets[actualIndex][i];
            // System.out.println("choosed " + hittingSet[hittingSetIndex]);
            
            // Create a copy of valid array before modifying
            int[] newValid = new int[valid.length];
            System.arraycopy(valid, 0, newValid, 0, valid.length);
            
            int result = algorithm1(args, map, removeNumberFromValid(newValid, map, hittingSet[hittingSetIndex]), hittingSet, hittingSetIndex+1, endTime);
            if(result == 1 || result == -1) return result;
            hittingSet[hittingSetIndex]=0;
        }
        
        return 0;
    }

    public static int[] alg2(Args args){
        int hittingSet[] = new int[args.k];

        long startTime = System.currentTimeMillis(); // current time in milliseconds
        long endTime = startTime + 3600 * 1000;      // add 3600 seconds (1 hour), converted to milliseconds

        int map[][] = mapper(args);

        int[] valid = new int[args.m];
        for(int i=0;i<args.m;i++)
            valid[i] = i+1;

        int result = algorithm2(args,map,valid,hittingSet,0,endTime);
        if(result ==-1){
            System.out.println("(2) Time limit");
            return new int[]{-1};
        }
        if(result==0)
            System.out.println("(2) failed");
        if(result==1){
            System.out.println("(2) Hitting Set:");
            for(int i=0;i<args.k;i++) System.out.print(hittingSet[i] + " ");
            System.out.println();
            return hittingSet;
        }
        return null;
    }

    public static int algorithm2(Args args, int[][] map, int[] valid, int[] hittingSet, int hittingSetIndex, long endTime){
        int validSubSets = countNonZero(valid);  // Replace length() with countNonZero()
        if(validSubSets<=0 && hittingSetIndex<= args.k) return 1;
        if(hittingSetIndex>=args.k) return 0;
        if(System.currentTimeMillis()>endTime) return -1;

        Random rand = new Random();
        int r=rand.nextInt(validSubSets); 
        
        // Find the actual subset index from random index
        int actualIndex = -1;
        int count=0;
        for(int i=0;i<valid.length;i++){
            if(valid[i]!=0){
                if(count == r){
                    actualIndex = valid[i]-1;  // Convert to 0-based index
                    break;
                }
                count++;
            }
        }
        commonSort(args.subsets[actualIndex], map, valid);

        for(int i=0;i<args.c && args.subsets[actualIndex][i]!=0;i++){
            hittingSet[hittingSetIndex] = args.subsets[actualIndex][i];
            // System.out.println("choosed " + hittingSet[hittingSetIndex]);

            // Create a copy of valid array before modifying
            int[] newValid = new int[valid.length];
            System.arraycopy(valid, 0, newValid, 0, valid.length);

            int result = algorithm2(args, map, removeNumberFromValid(newValid, map, hittingSet[hittingSetIndex]), hittingSet, hittingSetIndex+1, endTime);
            // System.out.println("back");
            if(result == 1 || result == -1) return result;
            hittingSet[hittingSetIndex]=0;
        }

        return 0;
    }

    public static int[] alg3(Args args){
        int hittingSet[] = new int[args.k];

        long startTime = System.currentTimeMillis(); // current time in milliseconds
        long endTime = startTime + 3600 * 1000;      // add 3600 seconds (1 hour), converted to milliseconds

        sortBasedOnSize(args.subsets);


        int map[][] = mapper(args);

        int[] valid = new int[args.m];
        for(int i=0;i<args.m;i++)
            valid[i] = i+1;
        
        int result = algorithm3(args,map,valid,hittingSet,0,endTime);
        if(result ==-1){
            System.out.println("(3) Time limit");
            return new int[]{-1};
        }
        if(result==0)
            System.out.println("(3) failed");
        if(result==1){
            System.out.println("(3) Hitting Set:");
            for(int i=0;i<args.k;i++) System.out.print(hittingSet[i] + " ");
            System.out.println();
            return hittingSet;
        }
        return null;
    }

    public static int algorithm3(Args args, int[][] map, int[] valid, int[] hittingSet, int hittingSetIndex, long endTime){
        int validSubSets = countNonZero(valid);  // Replace length() with countNonZero()
        if(validSubSets<=0 && hittingSetIndex<= args.k) return 1;
        if(hittingSetIndex>=args.k) return 0;
        if(System.currentTimeMillis()>endTime) return -1;
        
        int subsetId = valid[0] - 1;  // since valid[] is always kept packed at front

        for (int j = 0; j < args.c && args.subsets[subsetId][j] != 0; j++) {
            int e = args.subsets[subsetId][j];
            hittingSet[hittingSetIndex] = e;
            int[] newValid = removeNumberFromValid(Arrays.copyOf(valid, valid.length), map, e);
            int r = algorithm3(args, map, newValid, hittingSet, hittingSetIndex+1, endTime);
            if (r == 1 || r == -1) return r;
            hittingSet[hittingSetIndex] = 0;
        }
        return 0;
    }

    public static int[] alg4(Args args){
        int hittingSet[] = new int[args.k];

        long startTime = System.currentTimeMillis(); // current time in milliseconds
        long endTime = startTime + 3600 * 1000;      // add 3600 seconds (1 hour), converted to milliseconds


        sortBasedOnSize(args.subsets);
        int map[][] = mapper(args);

        int[] valid = new int[args.m];
        for(int i=0;i<args.m;i++)
            valid[i] = i+1;
        
        int result = algorithm4(args,map,valid,hittingSet,0,endTime);
        if(result ==-1){
            System.out.println("(4) Time limit");
        }
        if(result==0)
            System.out.println("(4) failed");
        if(result==1){
            System.out.println("(4) Hitting Set:");
            for(int i=0;i<args.k;i++) System.out.print(hittingSet[i] + " ");
            System.out.println();
            return hittingSet;
        }
        return null;
    }

    public static int algorithm4(Args args, int[][] map, int[] valid, int[] hittingSet, int hittingSetIndex, long endTime){
        int validSubSets = countNonZero(valid);  // Replace length() with countNonZero()
        if(validSubSets<=0 && hittingSetIndex<= args.k) return 1;
        if(hittingSetIndex>=args.k) return 0;
        if(System.currentTimeMillis()>endTime) return -1;
        
        int subsetId = valid[0] - 1;  // since valid[] is always kept packed at front

        commonSort(args.subsets[subsetId], map, valid);

        for(int j = 0; j < args.c && args.subsets[subsetId][j] != 0; j++){
            int e = args.subsets[subsetId][j];
            hittingSet[hittingSetIndex] = e;
            int[] newValid = removeNumberFromValid(Arrays.copyOf(valid, valid.length), map, e);
            int r = algorithm4(args, map, newValid, hittingSet, hittingSetIndex+1, endTime);
            if (r == 1 || r == -1) return r;
            hittingSet[hittingSetIndex] = 0;
        }
        return 0;
    }


    public static int[][] mapper(Args args){
        int map[][]=new int[args.n+1][args.m];
        for(int x=1;x<=args.n;x++){
            int y=0;
            for(int i=0;i<args.m;i++){
                for(int j=0;j<args.c;j++)
                    if(x==args.subsets[i][j]){
                        map[x][y]=i+1;
                        y++;
                    }
            }
        }
        return map;
    }

    public static Args copyArgs(Args original) {
        Args copy = new Args();
        copy.n = original.n;
        copy.m = original.m;
        copy.c = original.c;
        copy.k = original.k;
        copy.set = new int[copy.n];
        System.arraycopy(original.set, 0, copy.set, 0, copy.n);
        copy.subsets = new int[copy.m][copy.c];
        for(int i=0;i<copy.m;i++)
            System.arraycopy(original.subsets[i], 0, copy.subsets[i], 0, copy.c);
        return copy;
    }
    
    public static int countNonZero(int[] arr) {
        int count = 0;
        for(int i = 0; i < arr.length; i++) {
            if(arr[i]!=0) count++;
            else return count;
        }
        return count;
    }

    public static int[] swapArrays(int[][] arr, int cur, int dest) {
        if (cur < 0 || dest < 0 || cur >= arr.length || dest >= arr.length) {
            System.out.println("Warning: Invalid swap indices - cur: " + cur + ", dest: " + dest);
            return null;
        }
        int[] rtrnSet = arr[cur];      // store the current row
        arr[cur] = arr[dest];          // swap with the dest row
        arr[dest] = rtrnSet;           // complete the swap
        return rtrnSet;                // return the original (swapped) row
    }
    
    public static int[] removeNumberFromValid(int[] valid, int[][] map, int numberToAdd) {
        // Create a temporary array to hold the filtered result
        int[] temp = new int[valid.length];
        int tempIndex = 0;
        
        // For each valid subset index
        for(int i=0;i<valid.length;i++){
            boolean shouldRemove = false;
            // Check if the subset at valid[i] contains numberToAdd
            // We need to check if valid[i] appears in map[numberToAdd]
            for (int j=0;j<map[numberToAdd].length;j++){
                if(map[numberToAdd][j]== valid[i]){
                    // This subset contains the element, so we should remove it
                    shouldRemove = true;
                    break;
                }
            }
            
            // If the subset doesn't contain the element, keep it
            if (!shouldRemove) {
                temp[tempIndex] = valid[i];
                tempIndex++;
            }
        }
        
        return temp;
    }

    // this will return the array based on the commonnes on the valid subsets.
    public static void commonSort(int[] arr, int[][] map, int[] valid) {
        int[] commonness = new int[arr.length];
        
        for(int i=0;i<arr.length;i++){
            if(arr[i] == 0) continue;                                       // Skip if element is 0
            for(int j=0;j<map[arr[i]].length && map[arr[i]][j]!=0;j++){
                for(int x=0;x<valid.length;x++){
                    if(valid[x] == map[arr[i]][j]){
                        commonness[i]++;
                        break;                                              // Found this subset in valid, move to next subset
                    }
                }
            }
        }
        for(int i=0;i<arr.length;i++){                                      // Sort in descending order of commonness
            for(int j=0;j<arr.length-1-i;j++){
                if(commonness[j]<commonness[j+1]){
                    int tempCommon = commonness[j];
                    commonness[j] = commonness[j+1];
                    commonness[j+1] = tempCommon;
                    
                    int tempElement = arr[j];
                    arr[j] = arr[j+1];
                    arr[j+1] = tempElement;
                }
            }
        }
    }
    // this will sort all the subsets based on their length from low to high.
    public static void sortBasedOnSize(int[][] arr) {
        int[] counts = new int[arr.length];
        for(int i=0;i<arr.length;i++)
            counts[i] = countNonZero(arr[i]);
        
        for(int i=0;i<arr.length-1;i++){
            boolean swapped = false;
            for(int j=0;j<arr.length-1-i;j++){
                if(counts[j]>counts[j+1]){
                    int[] temp = arr[j];
                    arr[j] = arr[j+1];
                    arr[j+1] = temp;
                
                    int tempCount = counts[j];
                    counts[j] = counts[j+1];
                    counts[j+1] = tempCount;
                    
                    swapped = true;
                }
            }
            if (!swapped) break;
        }
    }

    public static void fwrite(String output, String fileName){
        String outputPath = fileName+".txt";
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(outputPath, true))) {
            writer.newLine();
            writer.write(output);
            // flush/close automatically by try-with-resources
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
