package com.skyetek.utility;

import java.util.* ;                                    
                              
/** Contains a collection of static methods related to Numbers. <p>

    
    - - - - - - - - - - - - - - - - - <p>
    
    Copyright (c) 1998-2000 Paul Wheaton <p>
    
    You are welcome to do whatever you want to with this source file provided
    that you maintain this comment fragment (between the dashed lines).
    Modify it, change the package name, change the class name ... personal or business 
    use ...  sell it, share it ... add a copyright for the portions you add ... <p>
    
    My goal in giving this away and maintaining the copyright is to hopefully direct 
    developers back to JavaRanch. <p>
    
    The original source can be found at <a href=http://www.javaranch.com>JavaRanch</a> <p>
    
    - - - - - - - - - - - - - - - - - <p>

    @author Paul Wheaton <p>
*/

public class Numbers
{

    /** Swap the upper and lower bytes of a two byte integer. <p>

        This is useful when you need to convert between Intel formatted integers(little endian) and Java/Motorola formatted
        integers(big endian). <p>

        @param x The number to be converted. <p>
        @return The converted number. <p>
    */
    public static short endian( short x )
    {
        int y = x >> 8 ;
        int z = ( x % 256 ) << 8 ;
        z |= y ;
        return (short)z ;
    }

    /** Reverse the byte order of a four byte integer. <p>

        <p>This is useful when you need to convert between Intel formatted integers(little endian) and Java/Motorola formatted
        integers(big endian). <p>

        If the original hex number was ABCD, the new hex number will be DCBA. <p>

        @param x The number to be converted. <p>
        @return The converted number. <p>
    */
    public static int endian( int x )
    {
        int a = x >>> 24 ;
        int b = ( x >>> 16 ) & 0xff ;
        int c = ( x >>> 8 ) & 0xff ;
        int d = x & 0xff ;
        x = ( d << 24 ) | ( c << 16 ) | ( b << 8 ) | a ;
        return x ;
    }

    /** Convert a possibly negative valued byte to a positive value integer. <p>

        A Java byte has a value of -128 to 127.  With eight bits and no sign, the negative numbers could be represented as
        a value of 128 to 255.  This method makes such a conversion, but stores the result in an integer since Java does
        not support unsigned bytes. <p>

        @param B The byte with a possibly negative value. <p>
        @return An integer with a value of 0 to 255. <p>
    */
    public static int byteToPositiveInt( byte b )
    {
        int i = b ;
        if ( i < 0 )
        {
            i += 256 ;
        }
        return i ;
    }

    /** Convert an integer possibly negative valued byte to a positive value integer. <p>

        A Java byte has a value of -128 to 127.  With eight bits and no sign, the negative numbers could be represented as
        a value of 128 to 255.  This method makes the conversion from 0-255 to -128-127. <p>

        This method assumes your integer will be of the range of 0 to 255.  Using a value outside of this range could
        generate an exception. <p>

        @param I The integer in the range of 0 to 255. <p>
        @return A byte with a possibly negative value. <p>
    */
    public static byte positiveIntToByte( int i )
    {
        System.out.println("deuce");
            if ( i > 127 )
            {
                    i -= 256 ;
            }
            return (byte) i ;
    }

    /** A convenience method to test if an integer is within a particular range. <p>

        @param x The value to test. <p>
        @param min The minimum value for x. <p>
        @param max The maximum value for x. <p>
        @return true if ( x >= min ) and ( x <= max ). <p>
    */
    public static boolean inRange( int x , int min , int max )
    {
        return ( ( x >= min ) && ( x <= max ) ) ;
    }

    /** A convenience method to test if a double is within a particular range. <p>

        @param x The value to test. <p>
        @param min The minimum value for x. <p>
        @param max The maximum value for x. <p>
        @return true if ( x >= min ) and ( x <= max ). <p>
    */
    public static boolean inRange( double x , double min , double max )
    {
        return ( ( x >= min ) && ( x <= max ) ) ;
    }

    /** A convenience method to test if a character is within a particular range. <p>

        @param x The character to test. <p>
        @param min The minimum value for x. <p>
        @param max The maximum value for x. <p>
        @return true if ( x >= min ) and ( x <= max ). <p>
    */
    public static boolean inRange( char x , char min , char max )
    {
        return ( ( x >= min ) && ( x <=max ) ) ;
    }

    private static char[] negs = { 'w' , 'W' , 's' , 'S' , '-' };

    /** Pass in a string that represents some form of lat or long and a decimal value is returned. <p>
    */
   /* public static double decimalDegrees( String text )
    {
        boolean negative = false ;
        Str s = new Str( text );
        s.replace( "EAST" , 'e' ); // these give a false "south"
        s.replace( "East" , 'e' );
        s.replace( "east" , 'e' );
        for( int i = 0 ; i < negs.length ; i++ )
        {
            if ( s.indexOf( negs[ i ] ) != -1 )
            {
                negative = true ;
            }
        }

        for( int i = 0 ; i < s.length() ; i++ )
        {
            char c = s.get( i );
            boolean numeric = false ;
            if ( Numbers.inRange( c , '0' , '9' ) )
            {
                numeric = true ;
            }
            else if ( c == '.' )
            {
                numeric = true ;
            }
            if ( ! numeric )
            {
                s.set( i , ' ' );
            }
        }

        s.removeDoubleSpaces();
        s.trim();

        double result = 0.0 ;
        if ( s.charCount(' ') > 0 )
        {
            // multiple numbers means some sort of degrees / minutes / seconds

            // degrees
            Str chunk = s.extractWord();
            result = Str.atod( chunk.toString() );

            // minutes
            chunk = s.extractWord();
            result += ( Str.atod( chunk.toString() ) / 60.0 ) ;

            // seconds
            if ( s.length() > 0 )
            {
                chunk = s.extractWord();
                result += ( Str.atod( chunk.toString() ) / 3600 ) ;
            }
        }
        else
        {
            // one big number
            result = Str.atod( s.toString() );
        }

        if ( negative )
        {
            result = 0 - result ;
        }
        return result ;
    }*/

    //public static void unitTest( TestLog log )
   // {
        // xxx need tests
    //}
    
}
