
        /**
         * Construct the stack.
         */
        template <class Object>
        stack<Object>::stack( int capacity ) : theArray( capacity )
        {
            topOfstack = -1;
        }

        /**
         * Test if the stack is logically empty.
         * Return true if empty, false otherwise.
         */
        template <class Object>
        bool stack<Object>::isEmpty( ) const
        {
            return topOfstack == -1;
        }

        /**
         * Test if the stack is logically full.
         * Return true if full, false otherwise.
         */
        template <class Object>
        bool stack<Object>::isFull( ) const
        {
            return topOfstack == theArray.size( ) - 1;
        }

        /**
         * Make the stack logically empty.
         */
        template <class Object>
        void stack<Object>::makeEmpty( )
        {
            topOfstack = -1;
        }

        /**
         * Get the most recently inserted item in the stack.
         * Does not alter the stack.
         * Return the most recently inserted item in the stack.
         * Exception Underflow if stack is already empty.
         */
        template <class Object>
        const Object & stack<Object>::top( ) const
        {
            if( isEmpty( ) )
                throw Underflow( );
            return theArray[ topOfstack ];
        }

        /**
         * Remove the most recently inserted item from the stack.
         * Exception Underflow if stack is already empty.
         */
        template <class Object>
        void stack<Object>::pop( )
        {
            if( isEmpty( ) )
                throw Underflow( );
            topOfstack--;
        }

        /**
         * Insert x into the stack, if not already full.
         * Exception Overflow if stack is already full.
         */
        template <class Object>
        void stack<Object>::push( const Object & x )
        {
            if( isFull( ) )
                throw Overflow( );
            theArray[ ++topOfstack ] = x;
        }

        /**
         * Return and remove most recently inserted item from the stack.
         * Return most recently inserted item.
         * Exception Underflow if stack is already empty.
         */
        template <class Object>
        Object stack<Object>::topAndPop( )
        {
            if( isEmpty( ) )
                throw Underflow( );
            return theArray[ topOfstack-- ];
        }
