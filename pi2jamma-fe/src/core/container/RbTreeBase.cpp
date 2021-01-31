#include "core/container/RbTreeBase.hpp"

RbTreeBase::RbTreeBase()
{
    reset();
}

RbTreeBase::~RbTreeBase()
{
    clear();
}

void RbTreeBase::reset()
{
    mSentinal.mpLeft = & mSentinal;
    mSentinal.mpRight = & mSentinal;
    mSentinal.mpParent = & mSentinal;
    mSentinal.mRed = 0;
    mRoot.mpLeft = & mSentinal;
    mRoot.mpRight = & mSentinal;
    mRoot.mpParent = & mSentinal;
    mRoot.mRed = 0; 
}

void RbTreeBase::clear()
{
    #ifdef DEBUG_CONTAINER
        auto pI = getFirst();
        while(pI)
        {
            pI->mpTree = nullptr;
            pI = getNext(pI);
        }
    #endif
 
    reset();
}

void RbTreeBase::insert(
	RbTreeNode* pItem,
	const InsertFunction& compare)
{
	#if 0
    ASSERT(
        ( ALLOW_DUPLICATES == OmAllowDuplicates_True ) ||
        OmIsNull( FindFirst( COMPARER::GetKey( pItem ) ) ) );
     #endif
    
    RbTreeNode * pY = nullptr;
    RbTreeNode * pX = pItem;

    #ifdef DEBUG_CONTAINER
        
        ASSERT( pX->mpTree == nullptr );
        pX->mpTree = this;
        
    #endif
    
    insertHelp(pX, compare);

    pX->mRed = 1;
  
    while( pX->mpParent->mRed )
    {
        if ( pX->mpParent == pX->mpParent->mpParent->mpLeft )
        {
            pY = pX->mpParent->mpParent->mpRight;
            
            if ( pY->mRed )
            {
                pX->mpParent->mRed = 0;
                pY->mRed = 0;
                pX->mpParent->mpParent->mRed = 1;
                pX = pX->mpParent->mpParent;
            }
            else
            {
                if (pX == pX->mpParent->mpRight)
                {
                    pX = pX->mpParent;
                    leftRotate( pX );
                }
                
                pX->mpParent->mRed = 0;
                pX->mpParent->mpParent->mRed = 1;
                rightRotate( pX->mpParent->mpParent );
            } 
        }
        else
        {
            pY = pX->mpParent->mpParent->mpLeft;
            
            if ( pY->mRed )
            {
                pX->mpParent->mRed = 0;
                pY->mRed = 0;
                pX->mpParent->mpParent->mRed = 1;
                pX = pX->mpParent->mpParent;
            }
            else
            {
                if ( pX == pX->mpParent->mpLeft )
                {
                    pX = pX->mpParent;
                    rightRotate( pX );
                }
                
                pX->mpParent->mRed = 0;
                pX->mpParent->mpParent->mRed = 1;
                leftRotate( pX->mpParent->mpParent );
            } 
        }
    }
    
    mRoot.mpLeft->mRed=0;
}

RbTreeNode* RbTreeBase::find(const CompareFunction& compare) const
{
    RbTreeNode* pX = mRoot.mpLeft;
    RbTreeNode* pSentinal= & mSentinal;
  
    int compVal;
    
    if ( pX == pSentinal )
    {
        return( 0 );
    }
    
    compVal = compare(pX);

    while( 0 != compVal )
    {
        if ( compVal >= 1 )
        {
            pX = pX->mpLeft;
        }
        else
        {
            pX = pX->mpRight;
        }
        
        if ( pX == pSentinal )
        {
            return( 0 );
        }
        
        compVal = compare(pX);
    }

    while( true )
    {
        RbTreeNode* pPrev = predecessor( pX );
    
        if ( pPrev == pSentinal )
        {
            break;
        }
        
        compVal = compare(pPrev);

        if ( compVal != 0 )
        {
            break;
        }
        
        pX = pPrev;
    }
    
    return pX;
}

void RbTreeBase::insertHelp(
	RbTreeNode* pZ,
	const InsertFunction& compare)
{
 
	RbTreeNode* pX = nullptr;
	RbTreeNode* pY = nullptr;
	RbTreeNode* pSentinal= & mSentinal;

	pZ->mpLeft = pZ->mpRight = pSentinal;
	pY = & mRoot;
	pX = mRoot.mpLeft;

    while( pX != pSentinal)
    {
        pY = pX;
        
        if ( /* 1 == */ 1 <= compare(pX,pZ))
        {
            pX=pX->mpLeft;
        }
        else
        {
            pX=pX->mpRight;
        }
    }

    pZ->mpParent = pY;

    if (
        ( pY == & mRoot) ||
        ( /* 1 == */ 1 <= compare(pY,pZ)))
    {
        pY->mpLeft = pZ;
    }
    else
    {
        pY->mpRight = pZ;
    }
}

void RbTreeBase::remove(RbTreeNode* pZ)
{

    RbTreeNode* pSentinal = & mSentinal;
    RbTreeNode* root = & mRoot;
    
    #ifdef DEBUG_CONTAINER
        
        ASSERT( pZ->mpTree == this );
        pZ->mpTree = NULL;
        
    #endif
        
    RbTreeNode* pY = ( (pZ->mpLeft == pSentinal ) || ( pZ->mpRight == pSentinal ) ) ? pZ : successor( pZ );
    
    RbTreeNode* pX = ( pY->mpLeft == pSentinal ) ? pY->mpRight : pY->mpLeft;
    
    if ( root == ( pX->mpParent = pY->mpParent ) )
    {
        root->mpLeft = pX;
    }
    else
    {
        if ( pY == pY->mpParent->mpLeft )
        {
            pY->mpParent->mpLeft = pX;
        }
        else
        {
            pY->mpParent->mpRight=pX;
        }
    }
  
    if ( pY != pZ )
    {
        if ( ! ( pY->mRed ) )
        {
            removeFixUp( pX );
        }
  
        pY->mpLeft = pZ->mpLeft;
        pY->mpRight = pZ->mpRight;
        pY->mpParent = pZ->mpParent;
        pY->mRed = pZ->mRed;
        pZ->mpLeft->mpParent = pZ->mpRight->mpParent = pY;
        
        if ( pZ == pZ->mpParent->mpLeft )
        {
            pZ->mpParent->mpLeft = pY; 
        }
        else
        {
            pZ->mpParent->mpRight=pY;
        }
    }
    else
    {
        if ( ! ( pY->mRed ) )
        {
            removeFixUp( pX );
        }
    }
}

void RbTreeBase::removeFixUp(RbTreeNode* pX)
{
    RbTreeNode* root = mRoot.mpLeft;
    RbTreeNode* w;

    while( ( ! pX->mRed ) && ( root != pX ) )
    {
        if ( pX == pX->mpParent->mpLeft )
        {
            w = pX->mpParent->mpRight;
      
            if ( w->mRed )
            {
                w->mRed = 0;
                pX->mpParent->mRed = 1;
                
                leftRotate( pX->mpParent );
                
                w = pX->mpParent->mpRight;
            }
            if ( ( ! w->mpRight->mRed ) && ( ! w->mpLeft->mRed ) )
            { 
                w->mRed = 1;
                pX = pX->mpParent;
            }
            else
            {
                if ( ! w->mpRight->mRed )
                {
                    w->mpLeft->mRed = 0;
                    w->mRed = 1;
      
                    rightRotate( w );
                    
                    w = pX->mpParent->mpRight;
                }
    
                w->mRed = pX->mpParent->mRed;
                pX->mpParent->mRed = 0;
                w->mpRight->mRed = 0;
                
                leftRotate( pX->mpParent );
                
                pX = root;
            }
        }
        else
        {
            w = pX->mpParent->mpLeft;
      
            if ( w->mRed )
            {
                w->mRed = 0;
                
                pX->mpParent->mRed = 1;
                
                rightRotate( pX->mpParent );
                
                w = pX->mpParent->mpLeft;
            }
            
            if ( ( ! w->mpRight->mRed ) && ( ! w->mpLeft->mRed ) )
            { 
                w->mRed = 1;
                pX = pX->mpParent;
            }
            else
            {
                if ( ! w->mpLeft->mRed )
                {
                    w->mpRight->mRed = 0;
                    w->mRed = 1;
                    
                    leftRotate( w );
                    
                    w = pX->mpParent->mpLeft;
                }
    
                w->mRed = pX->mpParent->mRed;
                pX->mpParent->mRed = 0;
                w->mpLeft->mRed = 0;
    
                rightRotate(pX->mpParent);
    
                pX=root;
            }
        }
    }
    
    pX->mRed=0;
}



void RbTreeBase::leftRotate(RbTreeNode* pX)
{
    RbTreeNode* pY;
    RbTreeNode* pSentinal = & mSentinal;

    pY = pX->mpRight;
    pX->mpRight = pY->mpLeft;

    if ( pY->mpLeft != pSentinal )
    {
        pY->mpLeft->mpParent=pX;
    }

    pY->mpParent = pX->mpParent;   

    if( pX == pX->mpParent->mpLeft)
    {
        pX->mpParent->mpLeft=pY;
    }
    else
    {
        pX->mpParent->mpRight=pY;
    }
    
    pY->mpLeft = pX;
    pX->mpParent = pY;
}

void RbTreeBase::rightRotate(RbTreeNode* pY)
{
    RbTreeNode* pX = nullptr;
    RbTreeNode* pSentinal= & mSentinal;

    pX = pY->mpLeft;
    pY->mpLeft = pX->mpRight;

    if ( pSentinal != pX->mpRight )
    {
        pX->mpRight->mpParent=pY;
    }
    
    pX->mpParent = pY->mpParent;
    
    if( pY == pY->mpParent->mpLeft )
    {
        pY->mpParent->mpLeft = pX;
    }
    else
    {
        pY->mpParent->mpRight = pX;
    }
    
    pX->mpRight = pY;
    pY->mpParent = pX;
}

RbTreeNode* RbTreeBase::successor(RbTreeNode* pX) {
    return
        const_cast<RbTreeNode*>(
            const_cast<const RbTreeBase*>(this)->successor(pX));
}

RbTreeNode* RbTreeBase::successor(RbTreeNode* pX) const
{ 
    RbTreeNode* pY = nullptr;
    RbTreeNode* pSentinal= & mSentinal;
    RbTreeNode* root=& mRoot;

    if ( pSentinal != ( pY = pX->mpRight) )
    {
        while( pY->mpLeft != pSentinal )
        { 
            pY = pY->mpLeft;
        }
        return( pY );
    }
    else
    {
        pY = pX->mpParent;
        
        while( pX == pY->mpRight )
        {
            pX = pY;
            pY = pY->mpParent;
        }
        
        if ( pY == root )
        {
            return( pSentinal );
        }
        
        return(pY);
    }
}

RbTreeNode* RbTreeBase::predecessor(RbTreeNode* pX) const
{
    RbTreeNode* pY = nullptr;
    RbTreeNode* pSentinal= & mSentinal;
    RbTreeNode* root=& mRoot;

    if ( pSentinal != ( pY = pX->mpLeft ) )
    {
        while( pY->mpRight != pSentinal )
        {
            pY = pY->mpRight;
        }
        
        return( pY );
    }
    else
    {
        pY = pX->mpParent;
    
        while( pX == pY->mpLeft )
        { 
            if ( pY == root )
            {
                return( pSentinal );
            }
            
            pX = pY;
            pY = pY->mpParent;
        }
    
        return( pY );
    }
}

RbTreeNode* RbTreeBase::getFirst() const
{
    RbTreeNode* pX = mRoot.mpLeft;
    RbTreeNode* pParent = 0;
    RbTreeNode* pSentinal = & mSentinal;
    
    while( pX != pSentinal )
    {
        pParent = pX;
        pX = pX->mpLeft;
    }
    
    return pParent;
}

RbTreeNode* RbTreeBase::getNext(RbTreeNode* pPrev) const
{
    ASSERT(nullptr != pPrev);

    RbTreeNode* pNext = successor(pPrev);
    
    if ( pNext == ( & mSentinal ) )
    {
        return 0;
    }
    
    return pNext;
}

RbTreeNode* RbTreeBase::findAt( size_t index ) const
{
    RbTreeNode* pS = getFirst();
    
    size_t count = 0;
    
    while(nullptr != pS)
    {
        if( count == index )
        {
            return pS;
        }
        
        pS = getNext( pS );
        
        count++;
    }
    
    ASSERT(false);
    
    return nullptr;
}

RbTreeNode* RbTreeBase::getLast(void) const
{
    RbTreeNode* pX = mRoot.mpLeft;
    RbTreeNode* pParent = nullptr;
    RbTreeNode* pSentinal = & mSentinal;
    
    while( pX != pSentinal )
    {
        pParent = pX;
        pX = pX->mpRight;
    }

    return pParent;
}

RbTreeNode* RbTreeBase::getPrev(RbTreeNode* pNext) const
{
    ASSERT(nullptr != pNext);
    
    RbTreeNode* pPrev = predecessor( pNext );
    
    if ( pPrev == ( & mSentinal ) )
    {
        return nullptr;
    }
    
    return pPrev;
}

size_t RbTreeBase::count() const
{
    RbTreeNode* pS = getFirst();
    
    size_t count = 0;
    
    while( nullptr != pS )
    {
        count++;
        pS = getNext( pS );
    }
    
    return count;
}

RbTreeNode* RbTreeBase::findClosest(
    const CompareFunction& compare) const
{
    RbTreeNode* pX = mRoot.mpLeft;
    RbTreeNode* pSentinal= & mSentinal;
  
    int compVal;
    
    if ( pX == pSentinal )
    {
        return( 0 );
    }
    
    compVal = compare(pX);
    
    while( 0 != compVal )
    {
        RbTreeNode* pPrev = pX;

        if ( compVal >= 1 )
        {
            pX = pX->mpLeft;
        }
        else
        {
            pX = pX->mpRight;
        }
        
        if ( pX == pSentinal )
        {
            return pPrev;
        }
        
        compVal = compare(pX);
    }

    while( true )
    {
        RbTreeNode* pPrev = predecessor( pX );
    
        if ( pPrev == pSentinal )
        {
            break;
        }
        
        compVal = compare(pPrev);

        if ( compVal != 0 )
        {
            break;
        }
        
        pX = pPrev;
    }
    
    return pX;
}

