#pragma once
#include "SFML\Graphics.hpp"
#include <vector>

// copied from https://github.com/SFML/SFML/wiki/Source:-Line-segment-with-thickness
class sfLine : public sf::Drawable
{
public:
	sfLine( const sf::Vector2f& point1, const sf::Vector2f& point2, sf::Color c, float thick ) :
		color( c ), thickness( thick )
	{
		sf::Vector2f direction = point2 - point1;
		sf::Vector2f unitDirection = direction / std::sqrt( direction.x*direction.x + direction.y*direction.y );
		sf::Vector2f unitPerpendicular( -unitDirection.y, unitDirection.x );

		sf::Vector2f offset = ( thickness / 2.f )*unitPerpendicular;

		vertices[0].position = point1 + offset;
		vertices[1].position = point2 + offset;
		vertices[2].position = point2 - offset;
		vertices[3].position = point1 - offset;

		for ( int i = 0; i<4; ++i )
			vertices[i].color = color;
	}

	void draw( sf::RenderTarget &target, sf::RenderStates states ) const
	{
		target.draw( vertices, 4, sf::Quads );
	}


private:
	sf::Vertex vertices[4];
	float thickness;
	sf::Color color;
};

// copied from https://github.com/SFML/SFML/wiki/Source:-line-with-thickness (modified for C++)
//float GetMagnitude( sf::Vector2f v )
//{
//	return (float)sqrt( v.x * v.x + v.y * v.y );
//}
//
//float Dot( sf::Vector2f v0, sf::Vector2f v1 )
//{
//	return v0.x * v1.x + v0.y * v1.y;
//}
//
//sf::Vector2f GetNormal( sf::Vector2f v )
//{
//	return sf::Vector2f( -v.y, v.x );
//}
//
//sf::Vector2f Normalized( sf::Vector2f v )
//{
//	return v / GetMagnitude(v);
//}
//
//sf::VertexArray GenerateTrianglesStrip( std::vector<sf::Vector2f> points, sf::Color color, float thickness )
//{
//	sf::VertexArray array( sf::TriangleStrip );
//	for ( unsigned i = 1; i < points.size() + 2; i++ )
//	{
//		sf::Vector2f v0 = points[( i - 1 ) % points.size()];
//		sf::Vector2f v1 = points[i % points.size()];
//		sf::Vector2f v2 = points[( i + 1 ) % points.size()];
//		sf::Vector2f v01 = Normalized( v1 - v0 );
//		sf::Vector2f v12 = Normalized( v2 - v1 );
//		sf::Vector2f d = GetNormal( v01 + v12 );
//		float dot = Dot( d, GetNormal( v01 ) );
//		d *= thickness / 2.f / dot; //< TODO: Add flat miter joint in extreme cases
//
//		array.append( sf::Vertex( v1 + d, color ) );
//		array.append( sf::Vertex( v1 - d, color ) );
//	}
//	return array;
//}