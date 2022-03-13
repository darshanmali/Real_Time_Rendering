//
//  MyView.m
//  01.Window
//
//  Created by Darshan Mali on 04/11/21.
//

#import "MyView.h"

@implementation MyView
{
    @private
    NSString *centralText;
}

-(id)initWithFrame:(CGRect)frame
{
        
    self = [super initWithFrame:frame];
    
    if(self)
    {
        centralText = @"Hello World !!!";
        
        UITapGestureRecognizer *singleTapGestureRecognizer = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(onSingleTap:)];
        [singleTapGestureRecognizer setNumberOfTapsRequired:1];
        [singleTapGestureRecognizer setNumberOfTouchesRequired:1];
        [singleTapGestureRecognizer setDelegate:self];
        [self addGestureRecognizer:singleTapGestureRecognizer];
        
        UITapGestureRecognizer *doubleTapGestureRecognizer = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(onDoubleTap:)];
        [doubleTapGestureRecognizer setNumberOfTapsRequired:2];
        [doubleTapGestureRecognizer setNumberOfTouchesRequired:1];
        [doubleTapGestureRecognizer setDelegate:self];
        [self addGestureRecognizer:doubleTapGestureRecognizer];
        
        [singleTapGestureRecognizer requireGestureRecognizerToFail:doubleTapGestureRecognizer];
        
        UISwipeGestureRecognizer *swipeGesturerecognizer = [[UISwipeGestureRecognizer alloc]initWithTarget:self action:@selector(onSwipe:)];
        [swipeGesturerecognizer setDelegate:self];
        [self addGestureRecognizer:swipeGesturerecognizer];
        
        UILongPressGestureRecognizer *longPressGesturerecognizer = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(onLongPress:)];
        [longPressGesturerecognizer setDelegate:self];
        [self addGestureRecognizer:longPressGesturerecognizer];
        
    }
    return (self);
}

-(void)drawRect:(CGRect)rect
{
    UIColor *backgroundColor = [UIColor blackColor];
    [backgroundColor set];
    UIRectFill(rect);
    
    NSDictionary *dictionaryWithTextAttribute = [NSDictionary dictionaryWithObjectsAndKeys:
                                                  [UIFont fontWithName:@"Helvetica" size:32.0], NSFontAttributeName,
                                                  [UIColor greenColor], NSForegroundColorAttributeName,
                                                  nil];
    
    CGSize textSize = [centralText sizeWithAttributes:dictionaryWithTextAttribute];
    
    CGPoint point;
    point.x = (rect.size.width/2) - (textSize.width/2);
    point.y = (rect.size.height/2) - (textSize.height/2);
    
    [centralText drawAtPoint:point withAttributes:dictionaryWithTextAttribute];

}

-(void)onSingleTap:(UITapGestureRecognizer *)gr
{
    //code
    centralText = @"Single Tap";
    
    [self setNeedsDisplay];
}

-(void)onDoubleTap:(UITapGestureRecognizer *)gr
{
    //code
    centralText = @"Double Tap";
    
    [self setNeedsDisplay];
}

-(void)onSwipe:(UISwipeGestureRecognizer *)gr
{
    //code
    [self release];
    
    exit (0);
}

-(void)onLongPress:(UILongPressGestureRecognizer *)gr
{
    //code
    centralText = @"Long Press";
    
    [self setNeedsDisplay];
    
}

-(void)dealloc
{
    //code
    [super dealloc];
}
@end
